/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mecavus <mecavus@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 12:00:00 by mecavus          #+#    #+#             */
/*   Updated: 2025/07/10 12:00:00 by mecavus         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <sys/types.h>
#include <sys/wait.h>

int	is_builtin(char *command)
{
	if (!command)
		return (0);
	if (!ft_strcmp(command, "echo") || !ft_strcmp(command, "cd")
		|| !ft_strcmp(command, "pwd") || !ft_strcmp(command, "export")
		|| !ft_strcmp(command, "unset") || !ft_strcmp(command, "env")
		|| !ft_strcmp(command, "exit"))
		return (1);
	return (0);
}

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

char	*find_command_path(char *command, t_env *env_list)
{
	char	**paths;
	char	*path_env;
	char	*full_path;
	char	*temp;
	int		i;

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
			int j = 0;
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

void	execute_external(char **args, t_env *env_list)
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
		if (execve(cmd_path, args, env_array) == -1)
		{
			perror("minishell");
			cmd_path = NULL;
			int i = 0;
			while (env_array[i])
				env_array[i++] = NULL;
			env_array = NULL;
			exit(126);
		}
	}
	else if (pid > 0)
	{
		cmd_path = NULL;
		free_env_array(env_array);
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			exit_status(WEXITSTATUS(status), PUSH);
	}
}

void	execute_command(char **args, t_env *env_list)
{
	if (is_builtin(args[0]))
		execute_builtin(args, &env_list);
	else
		execute_external(args, env_list);
}
