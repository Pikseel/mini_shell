/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mecavus <mecavus@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 12:00:00 by mecavus           #+#    #+#             */
/*   Updated: 2025/07/21 15:43:34 by mecavus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <sys/wait.h>

static void	setup_pipes(t_command *cmd_list)
{
	int			pipe_fd[2];
	t_command	*current;
	t_command	*next;
	t_main		*shell;

	current = cmd_list;
	while (current && current->next)
	{
		next = current->next;
		if (pipe(pipe_fd) == -1)
		{
			shell = (t_main *)ft_malloc(0, GET_SHELL);
			clear_exit(shell, 1, "pipe error");
		}
		if (current->output_fd == STDOUT_FILENO)
			current->output_fd = pipe_fd[1];
		if (next->input_fd == STDIN_FILENO)
			next->input_fd = pipe_fd[0];
		current = next;
	}
}

static void	setup_child_fds(t_command *current)
{
	if (current->input_fd != STDIN_FILENO)
	{
		dup2(current->input_fd, STDIN_FILENO);
		close(current->input_fd);
	}
	if (current->output_fd != STDOUT_FILENO)
	{
		dup2(current->output_fd, STDOUT_FILENO);
		close(current->output_fd);
	}
}

static void	close_unused_fds(t_command *cmd_list, t_command *current)
{
	t_command	*close_cmd;

	close_cmd = cmd_list;
	while (close_cmd)
	{
		if (close_cmd != current && close_cmd->input_fd != STDIN_FILENO)
			close(close_cmd->input_fd);
		if (close_cmd != current && close_cmd->output_fd != STDOUT_FILENO)
			close(close_cmd->output_fd);
		close_cmd = close_cmd->next;
	}
}

void	execute_piped_child(t_command *cmd_list, t_command *current,
	t_env *env_list)
{
	if (current->redirect_failed)
		clear_exit(NULL, 1, NULL);
	setup_child_fds(current);
	close_unused_fds(cmd_list, current);
	if (!current || !current->args || !current->args[0])
		clear_exit(NULL, 1, NULL);
	if (is_builtin(current->args[0]))
		execute_builtin(current->args, &env_list);
	else
		execute_external_piped(current->args, env_list);
	clear_exit(NULL, exit_status(0, PULL), NULL);
}

void	execute_piped_commands(t_command *cmd_list, t_env *env_list)
{
	pid_t	*pids;
	int		cmd_count;

	cmd_count = count_commands(cmd_list);
	pids = ft_malloc(sizeof(pid_t) * cmd_count, ALLOC);
	setup_pipes(cmd_list);
	fork_all_processes(cmd_list, pids, env_list);
	close_all_pipes(cmd_list);
	wait_all_processes(pids, cmd_count);
	pids = NULL;
}
