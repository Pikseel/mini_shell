/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_external.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mecavus <mecavus@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 12:36:36 by mecavus           #+#    #+#             */
/*   Updated: 2025/07/24 10:13:22 by mecavus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>
#include <sys/wait.h>

static void	handle_signal_status(int status)
{
	if (WTERMSIG(status) == SIGINT)
	{
		ft_putstr_fd("\n", 1);
		exit_status(130, PUSH);
	}
	else if (WTERMSIG(status) == SIGQUIT)
	{
		ft_putstr_fd("Quit (core dumped)\n", 1);
		exit_status(131, PUSH);
	}
}

static void	wait_and_handle_status(pid_t pid)
{
	int	status;

	ignore_signal();
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		exit_status(WEXITSTATUS(status), PUSH);
	else if (WIFSIGNALED(status))
		handle_signal_status(status);
	init_signal();
}

static void	handle_child_process(char **args, char *cmd_path, char **env_array)
{
	execve_signal();
	if (execve(cmd_path, args, env_array) == -1)
	{
		perror("minishell");
		cmd_path = NULL;
		clear_exit(NULL, 126, NULL);
	}
}

void	execute_external(char **args, t_env *env_list)
{
	pid_t	pid;
	char	**env_array;
	char	*cmd_path;

	cmd_path = find_command_path(args[0], env_list);
	if (!cmd_path)
	{
		ft_putstr_fd("minishell: command not found: ", 2);
		ft_putstr_fd(args[0], 2);
		ft_putstr_fd("\n", 2);
		exit_status(127, PUSH);
		return ;
	}
	env_array = env_list_to_array(env_list);
	pid = fork();
	if (pid == 0)
		handle_child_process(args, cmd_path, env_array);
	else if (pid > 0)
	{
		cmd_path = NULL;
		wait_and_handle_status(pid);
	}
}

void	execute_external_piped(char **args, t_env *env_list)
{
	char	**env_array;
	char	*cmd_path;

	cmd_path = find_command_path(args[0], env_list);
	if (!cmd_path)
	{
		ft_putstr_fd("minishell: command not found: ", 2);
		ft_putstr_fd(args[0], 2);
		ft_putstr_fd("\n", 2);
		clear_exit(NULL, 127, NULL);
	}
	env_array = env_list_to_array(env_list);
	execve_signal();
	if (execve(cmd_path, args, env_array) == -1)
	{
		perror("minishell");
		clear_exit(NULL, 126, NULL);
	}
}
