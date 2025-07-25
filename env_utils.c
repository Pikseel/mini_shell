/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mecavus <mecavus@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 14:25:00 by emrozmen          #+#    #+#             */
/*   Updated: 2025/07/24 10:09:02 by mecavus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>

static void	init_default_env(t_env **env)
{
	char	*path;
	char	*pwd;
	char	*pwd_copy;

	pwd = getcwd(NULL, 0);
	pwd_copy = ft_strdup(pwd);
	free(pwd);
	path = "/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin";
	lst_addback(env, lst_new(ft_strdup("OLDPWD"), NULL));
	lst_addback(env, lst_new(ft_strdup("PATH"), ft_strdup(path)));
	lst_addback(env, lst_new(ft_strdup("PWD"), pwd_copy));
	lst_addback(env, lst_new(ft_strdup("SHLVL"), ft_strdup("1")));
	lst_addback(env, lst_new(ft_strdup("_"), ft_strdup("]")));
}

static void	parse_env_entry(char *env_entry, char **key, char **value)
{
	char	*equal;

	equal = ft_strchr(env_entry, '=');
	if (equal)
	{
		*key = ft_substr(env_entry, 0, equal - env_entry);
		*value = ft_strdup(equal + 1);
	}
	else
	{
		*key = ft_strdup(env_entry);
		*value = NULL;
	}
}

t_env	*init_env_list(char **env)
{
	t_env	*env_list;
	char	*key;
	char	*value;
	int		flag;
	int		shlvl;

	env_list = NULL;
	if (!env || !*env)
		init_default_env(&env_list);
	while (env && *env)
	{
		parse_env_entry(*env, &key, &value);
		if (ft_strcmp(key, "SHLVL") == 0 && value)
		{
			flag = 0;
			shlvl = ft_atoi(value, &flag) + 1;
			value = ft_itoa(shlvl);
		}
		lst_addback(&env_list, lst_new(key, value));
		env++;
	}
	return (env_list);
}

static int	count_env(t_env *env_list)
{
	int	count;

	count = 0;
	while (env_list)
	{
		count++;
		env_list = env_list->next;
	}
	return (count);
}

char	**env_list_to_array(t_env *env_list)
{
	char	**env_array;
	char	*temp;
	int		count;
	int		i;

	count = count_env(env_list);
	env_array = ft_malloc(sizeof(char *) * (count + 1), ALLOC);
	i = 0;
	while (env_list)
	{
		if (env_list->value)
		{
			temp = ft_strjoin(env_list->key, "=");
			env_array[i] = ft_strjoin(temp, env_list->value);
			i++;
		}
		env_list = env_list->next;
	}
	env_array[i] = NULL;
	return (env_array);
}
