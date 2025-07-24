/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redirections.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mecavus <mecavus@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 15:50:00 by mecavus           #+#    #+#             */
/*   Updated: 2025/07/24 09:58:03 by mecavus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <fcntl.h>
#include <stdio.h>

void	handle_input_redirection(t_token *current, t_command *cmd)
{
	int	fd;

	if (current->next && current->next->type == R_FILE)
	{
		fd = open(current->next->value, O_RDONLY);
		if (fd == -1)
		{
			perror("minishell");
			cmd->redirect_failed = 1;
			exit_status(1, PUSH);
			return ;
		}
		if (cmd->input_fd != STDIN_FILENO)
			close(cmd->input_fd);
		cmd->input_fd = fd;
		current->next->is_removed = 1;
	}
	current->is_removed = 1;
}

void	handle_output_redirection(t_token *current, t_command *cmd)
{
	int	fd;

	if (current->next && current->next->type == R_FILE)
	{
		fd = open(current->next->value, O_CREAT | O_WRONLY | O_TRUNC, 0644);
		if (fd == -1)
		{
			perror("minishell");
			cmd->redirect_failed = 1;
			exit_status(1, PUSH);
			return ;
		}
		if (cmd->output_fd != STDOUT_FILENO)
			close(cmd->output_fd);
		cmd->output_fd = fd;
		current->next->is_removed = 1;
	}
	current->is_removed = 1;
}

void	handle_append_redirection(t_token *current, t_command *cmd)
{
	int	fd;

	if (current->next && current->next->type == R_FILE)
	{
		fd = open(current->next->value, O_CREAT | O_WRONLY | O_APPEND, 0644);
		if (fd == -1)
		{
			perror("minishell");
			cmd->redirect_failed = 1;
			exit_status(1, PUSH);
			return ;
		}
		if (cmd->output_fd != STDOUT_FILENO)
			close(cmd->output_fd);
		cmd->output_fd = fd;
		current->next->is_removed = 1;
	}
	current->is_removed = 1;
}

void	handle_heredoc_redirection(t_token *current, t_command *cmd,
				t_env *env_list)
{
	int	fd;

	if (current->next && current->next->type == HERKEY)
	{
		fd = handle_heredoc(current, env_list);
		if (fd == -1)
		{
			exit_status(130, PUSH);
			cmd->redirect_failed = 1;
			return ;
		}
		if (cmd->input_fd != STDIN_FILENO)
			close(cmd->input_fd);
		cmd->input_fd = fd;
		current->next->is_removed = 1;
	}
	current->is_removed = 1;
}

void	handle_redirections(t_token *current, t_command *cmd,
		t_env *env_list)
{
	while (current && current->type != PIPE)
	{
		if (current->type == R_IN)
			handle_input_redirection(current, cmd);
		else if (current->type == R_OUT)
			handle_output_redirection(current, cmd);
		else if (current->type == APPEND)
			handle_append_redirection(current, cmd);
		else if (current->type == HERDOC)
			handle_heredoc_redirection(current, cmd, env_list);
		if (cmd->redirect_failed)
			return ;
		current = current->next;
	}
}
