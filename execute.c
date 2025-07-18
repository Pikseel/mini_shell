/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mecavus <mecavus@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 12:00:00 by mecavus           #+#    #+#             */
/*   Updated: 2025/07/18 17:11:58 by mecavus          ###   ########.fr       */
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

static char	*find_command_path(char *command, t_env *env_list)
{
	char	**paths;
	char	*path_env;
	char	*full_path;
	char	*temp;
	int		i;
	int		j;

	if (ft_strchr(command, '/'))
		return (ft_strdup(command));
	path_env = get_env_value(env_list, "PATH");
	if (!path_env)
		return (NULL);
	paths = ft_split_sep(path_env, ':');
	i = 0;
	while (paths[i])
	{
		temp = ft_strjoin(paths[i], "/");
		full_path = ft_strjoin(temp, command);
		temp = NULL;
		if (access(full_path, X_OK) == 0)
		{
			j = 0;
			while (paths[j])
				paths[j++] = NULL;
			paths = NULL;
			return (full_path);
		}
		full_path = NULL;
		i++;
	}
	i = 0;
	while (paths[i])
		paths[i++] = NULL;
	paths = NULL;
	return (NULL);
}

static void	free_env_array(char **env_array)
{
	int	i;

	if (!env_array)
		return ;
	i = 0;
	while (env_array[i])
	{
		env_array[i] = NULL;
		i++;
	}
	env_array = NULL;
}

static void	execute_external(char **args, t_env *env_list)
{
	pid_t	pid;
	char	**env_array;
	char	*cmd_path;
	int		status;

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
	{
		execve_signal();
		if (execve(cmd_path, args, env_array) == -1)
		{
			perror("minishell");
			cmd_path = NULL;
			free_env_array(env_array);
			clear_exit(NULL, 126, NULL);
		}
	}
	else if (pid > 0)
	{
		cmd_path = NULL;
		free_env_array(env_array);
		ignore_signal();
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			exit_status(WEXITSTATUS(status), PUSH);
		else if (WIFSIGNALED(status))
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
		init_signal();
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

void	execute_command(t_command *cmd, t_env *env_list)
{
	int	saved_stdin;
	int	saved_stdout;
	
	if (!cmd || !cmd->args || !cmd->args[0])
		return ;
	
	// Save original stdin/stdout
	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	
	// Apply input redirection
	if (cmd->input_fd != STDIN_FILENO)
	{
		dup2(cmd->input_fd, STDIN_FILENO);
		close(cmd->input_fd);
	}
	
	// Apply output redirection
	if (cmd->output_fd != STDOUT_FILENO)
	{
		dup2(cmd->output_fd, STDOUT_FILENO);
		close(cmd->output_fd);
	}
	
	if (!ft_strcmp(cmd->args[0], "echo") || !ft_strcmp(cmd->args[0], "cd")
		|| !ft_strcmp(cmd->args[0], "pwd") || !ft_strcmp(cmd->args[0], "export")
		|| !ft_strcmp(cmd->args[0], "unset") || !ft_strcmp(cmd->args[0], "env")
		|| !ft_strcmp(cmd->args[0], "exit"))
		execute_builtin(cmd->args, &env_list);
	else
		execute_external(cmd->args, env_list);
	
	// Restore original stdin/stdout
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdin);
	close(saved_stdout);
}
