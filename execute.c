/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mecavus <mecavus@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 12:00:00 by mecavus           #+#    #+#             */
/*   Updated: 2025/07/21 19:35:05 by mecavus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <sys/types.h>
#include <sys/wait.h>

void	execute_builtin(char **args, t_env **env_list)
{
	if (!ft_strcmp(args[0], "echo"))
		builtin_echo(args);
	else if (!ft_strcmp(args[0], "cd"))
		builtin_cd(args, *env_list);
	else if (!ft_strcmp(args[0], "pwd"))
		builtin_pwd();
	else if (!ft_strcmp(args[0], "export"))
		builtin_export(args, env_list);
	else if (!ft_strcmp(args[0], "unset"))
		builtin_unset(args, env_list);
	else if (!ft_strcmp(args[0], "env"))
		builtin_env(*env_list);
	else if (!ft_strcmp(args[0], "exit"))
		builtin_exit(args);
}

int	is_builtin(char *command)
{
	if (!ft_strcmp(command, "echo") || !ft_strcmp(command, "cd")
		|| !ft_strcmp(command, "pwd") || !ft_strcmp(command, "export")
		|| !ft_strcmp(command, "unset") || !ft_strcmp(command, "env")
		|| !ft_strcmp(command, "exit"))
		return (1);
	return (0);
}

static void	setup_redirections(t_command *cmd, int *saved_stdin,
	int *saved_stdout)
{
	*saved_stdin = dup(STDIN_FILENO);
	*saved_stdout = dup(STDOUT_FILENO);
	if (cmd->input_fd != STDIN_FILENO)
	{
		dup2(cmd->input_fd, STDIN_FILENO);
		close(cmd->input_fd);
	}
	if (cmd->output_fd != STDOUT_FILENO)
	{
		dup2(cmd->output_fd, STDOUT_FILENO);
		close(cmd->output_fd);
	}
}

static void	restore_redirections(int saved_stdin, int saved_stdout)
{
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdin);
	close(saved_stdout);
}

void	execute_command(t_command *cmd, t_env **env_list)
{
	int	saved_stdin;
	int	saved_stdout;

	if (!cmd || !cmd->args || !cmd->args[0])
		return ;
	if (cmd->redirect_failed)
		return ;
	setup_redirections(cmd, &saved_stdin, &saved_stdout);
	if (is_builtin(cmd->args[0]))
		execute_builtin(cmd->args, env_list);
	else
		execute_external(cmd->args, *env_list);
	restore_redirections(saved_stdin, saved_stdout);
}
