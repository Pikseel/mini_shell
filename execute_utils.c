/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mecavus <mecavus@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 12:18:35 by mecavus           #+#    #+#             */
/*   Updated: 2025/07/21 15:43:33 by mecavus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <sys/types.h>
#include <sys/wait.h>

static void	free_paths(char **paths)
{
	int	i;

	if (!paths)
		return ;
	i = 0;
	while (paths[i])
		paths[i++] = NULL;
	paths = NULL;
}

static char	*try_path(char *path, char *command)
{
	char	*temp;
	char	*full_path;

	temp = ft_strjoin(path, "/");
	full_path = ft_strjoin(temp, command);
	temp = NULL;
	if (access(full_path, X_OK) == 0)
		return (full_path);
	full_path = NULL;
	return (NULL);
}

char	*find_command_path(char *command, t_env *env_list)
{
	char	**paths;
	char	*path_env;
	char	*full_path;
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
		full_path = try_path(paths[i], command);
		if (full_path)
		{
			free_paths(paths);
			return (full_path);
		}
		i++;
	}
	free_paths(paths);
	return (NULL);
}

void	free_env_array(char **env_array)
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
