/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mecavus <mecavus@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 12:00:00 by mecavus          #+#    #+#             */
/*   Updated: 2025/07/10 12:00:00 by mecavus         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <sys/wait.h>

void	setup_pipes(t_command *cmd_list)
{
	int			pipe_fd[2];
	t_command	*current;
	t_command	*next;

	current = cmd_list;
	while (current && current->next)
	{
		next = current->next;
		if (pipe(pipe_fd) == -1)
		{
			perror("minishell: pipe");
			return ;
		}
		current->output_fd = pipe_fd[1];
		next->input_fd = pipe_fd[0];
		current = next;
	}
}

void	execute_piped_commands(t_command *cmd_list, t_env *env_list)
{
	pid_t		*pids;
	t_command	*current;
	int			cmd_count;
	int			i;
	int			status;

	cmd_count = 0;
	current = cmd_list;
	while (current)
	{
		cmd_count++;
		current = current->next;
	}
	pids = ft_malloc(sizeof(pid_t) * cmd_count, ALLOC);
	setup_pipes(cmd_list);
	i = 0;
	current = cmd_list;
	while (current)
	{
		pids[i] = fork();
		if (pids[i] == 0)
		{
			init_child_signal();
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
			t_command *close_cmd = cmd_list;
			while (close_cmd)
			{
				if (close_cmd != current && close_cmd->input_fd != STDIN_FILENO)
					close(close_cmd->input_fd);
				if (close_cmd != current && close_cmd->output_fd != STDOUT_FILENO)
					close(close_cmd->output_fd);
				close_cmd = close_cmd->next;
			}
			execute_command(current->args, env_list);
			exit(exit_status(0, PULL));
		}
		i++;
		current = current->next;
	}
	current = cmd_list;
	while (current)
	{
		if (current->input_fd != STDIN_FILENO)
			close(current->input_fd);
		if (current->output_fd != STDOUT_FILENO)
			close(current->output_fd);
		current = current->next;
	}
	for (i = 0; i < cmd_count; i++)
	{
		waitpid(pids[i], &status, 0);
		if (i == cmd_count - 1 && WIFEXITED(status))
			exit_status(WEXITSTATUS(status), PUSH);
	}
}
