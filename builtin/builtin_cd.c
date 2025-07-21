/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mecavus <mecavus@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 13:00:00 by mecavus           #+#    #+#             */
/*   Updated: 2025/07/21 19:35:12 by mecavus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	update_env_value(t_env *env_list, char *key, char *new_value)
{
	while (env_list)
	{
		if (ft_strcmp(env_list->key, key) == 0)
		{
			env_list->value = ft_strdup(new_value);
			return ;
		}
		env_list = env_list->next;
	}
}

static char	*get_target_directory(char **args, t_env *env_list)
{
	if (!args[1] || ft_strcmp(args[1], "~") == 0)
		return (get_env_value(env_list, "HOME"));
	else if (ft_strcmp(args[1], "-") == 0)
		return (get_env_value(env_list, "OLDPWD"));
	else
		return (args[1]);
}

static int	handle_cd_error(char **args, char *target_dir, char *old_pwd)
{
	if (!target_dir)
	{
		ft_putstr_fd("minishell: cd: ", 2);
		ft_putstr_fd(args[1], 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		exit_status(1, PUSH);
		return (1);
	}
	if (chdir(target_dir) != 0)
	{
		perror("minishell: cd");
		if (old_pwd)
			free(old_pwd);
		exit_status(1, PUSH);
		return (1);
	}
	return (0);
}

void	builtin_cd(char **args, t_env *env_list)
{
	char	*target_dir;
	char	*old_pwd;
	char	*new_pwd;

	target_dir = get_target_directory(args, env_list);
	old_pwd = getcwd(NULL, 0);
	if (handle_cd_error(args, target_dir, old_pwd))
		return ;
	new_pwd = getcwd(NULL, 0);
	update_env_value(env_list, "OLDPWD", old_pwd);
	update_env_value(env_list, "PWD", new_pwd);
	free(old_pwd);
	free(new_pwd);
	exit_status(0, PUSH);
}
