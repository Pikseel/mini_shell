/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mecavus <mecavus@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 13:53:11 by mecavus           #+#    #+#             */
/*   Updated: 2025/07/24 09:57:34 by mecavus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <sys/wait.h>

int	count_commands(t_command *cmd_list)
{
	int			count;
	t_command	*current;

	count = 0;
	current = cmd_list;
	while (current)
	{
		count++;
		current = current->next;
	}
	return (count);
}

void	fork_all_processes(t_command *cmd_list, pid_t *pids, t_env *env_list)
{
	t_command	*current;
	int			i;

	i = 0;
	current = cmd_list;
	while (current)
	{
		pids[i] = fork();
		if (pids[i] == 0)
			execute_piped_child(cmd_list, current, env_list);
		i++;
		current = current->next;
	}
}

void	close_all_pipes(t_command *cmd_list)
{
	t_command	*current;

	current = cmd_list;
	while (current)
	{
		if (current->input_fd != STDIN_FILENO)
			close(current->input_fd);
		if (current->output_fd != STDOUT_FILENO)
			close(current->output_fd);
		current = current->next;
	}
}

void	wait_all_processes(pid_t *pids, int cmd_count)
{
	int	i;
	int	status;

	i = 0;
	while (i < cmd_count)
	{
		waitpid(pids[i], &status, 0);
		if (i == cmd_count - 1 && WIFEXITED(status))
			exit_status(WEXITSTATUS(status), PUSH);
		i++;
	}
}
