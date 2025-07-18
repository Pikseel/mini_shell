/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mecavus <mecavus@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 12:00:00 by mecavus           #+#    #+#             */
/*   Updated: 2025/07/18 15:40:14 by mecavus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	count_word_tokens(t_token *tokens)
{
	int		count;
	t_token	*current;

	count = 0;
	current = tokens;
	while (current && current->type != PIPE)
	{
		if (current->type == WORD && current->is_removed == 0)
			count++;
		current = current->next;
	}
	return (count);
}

static t_command	*create_command(void)
{
	t_command	*cmd;

	cmd = ft_malloc(sizeof(t_command), ALLOC);
	cmd->command = NULL;
	cmd->args = NULL;
	cmd->input_fd = STDIN_FILENO;
	cmd->output_fd = STDOUT_FILENO;
	cmd->next = NULL;
	return (cmd);
}

static void	handle_redirections(t_token *current, t_command *cmd, t_env *env_list)
{
	int	fd;

	while (current && current->type != PIPE)
	{
		if (current->type == R_IN)
		{
			if (current->next && current->next->type == R_FILE)
			{
				fd = open(current->next->value, O_RDONLY);
				if (fd == -1)
				{
					perror("minishell");
					return ;
				}
				if (cmd->input_fd != STDIN_FILENO)
					close(cmd->input_fd);
				cmd->input_fd = fd;
				current->next->is_removed = 1;
			}
			current->is_removed = 1;
		}
		else if (current->type == R_OUT)
		{
			if (current->next && current->next->type == R_FILE)
			{
				fd = open(current->next->value, O_CREAT | O_WRONLY | O_TRUNC, 0644);
				if (fd == -1)
				{
					perror("minishell");
					return ;
				}
				if (cmd->output_fd != STDOUT_FILENO)
					close(cmd->output_fd);
				cmd->output_fd = fd;
				current->next->is_removed = 1;
			}
			current->is_removed = 1;
		}
		else if (current->type == APPEND)
		{
			if (current->next && current->next->type == R_FILE)
			{
				fd = open(current->next->value, O_CREAT | O_WRONLY | O_APPEND, 0644);
				if (fd == -1)
				{
					perror("minishell");
					return ;
				}
				if (cmd->output_fd != STDOUT_FILENO)
					close(cmd->output_fd);
				cmd->output_fd = fd;
				current->next->is_removed = 1;
			}
			current->is_removed = 1;
		}
		else if (current->type == HERDOC)
		{
			if (current->next && current->next->type == HERKEY)
			{
				fd = handle_heredoc(current, env_list);
				if (fd == -1)
				{
					exit_status(130, PUSH);
					return ;
				}
				if (cmd->input_fd != STDIN_FILENO)
					close(cmd->input_fd);
				cmd->input_fd = fd;
				current->next->is_removed = 1;
			}
			current->is_removed = 1;
		}
		current = current->next;
	}
}

static void	add_command(t_command **cmd_list, t_command *new_cmd)
{
	t_command	*current;

	if (!*cmd_list)
	{
		*cmd_list = new_cmd;
		return ;
	}
	current = *cmd_list;
	while (current->next)
		current = current->next;
	current->next = new_cmd;
}

t_command	*parse_tokens_to_commands(t_token *tokens, t_env *env_list)
{
	t_command	*cmd_list;
	t_command	*current_cmd;
	t_token		*current_tkn;
	t_token		*start_tkn;
	int			ac;
	int			i;

	cmd_list = NULL;
	current_tkn = tokens;
	while (current_tkn)
	{
		current_cmd = create_command();
		start_tkn = current_tkn;
		
		// Handle redirections first
		handle_redirections(current_tkn, current_cmd, env_list);
		
		// Count remaining word tokens from start
		ac = count_word_tokens(start_tkn);
		if (ac > 0)
		{
			current_cmd->args = ft_malloc(sizeof(char *) * (ac + 1), ALLOC);
			i = 0;
			// Reset to start and collect words
			current_tkn = start_tkn;
			while (current_tkn && current_tkn->type != PIPE)
			{
				if (current_tkn->type == WORD && current_tkn->is_removed == 0 && i < ac)
				{
					current_cmd->args[i++] = ft_strdup(current_tkn->value);
					if (!current_cmd->command)
						current_cmd->command = ft_strdup(current_tkn->value);
				}
				current_tkn = current_tkn->next;
			}
			current_cmd->args[i] = NULL;
		}
		else
		{
			// No args, just skip to next pipe
			current_tkn = start_tkn;
			while (current_tkn && current_tkn->type != PIPE)
				current_tkn = current_tkn->next;
		}
		
		add_command(&cmd_list, current_cmd);
		if (current_tkn && current_tkn->type == PIPE)
			current_tkn = current_tkn->next;
	}
	return (cmd_list);
}
