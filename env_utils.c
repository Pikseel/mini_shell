/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emrozmen <emrozmen@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 14:25:00 by emrozmen          #+#    #+#             */
/*   Updated: 2025/07/15 13:40:32 by emrozmen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

	env_list = NULL;
	if (!env || !*env)
		init_default_env(&env_list);
	while (env && *env)
	{
		parse_env_entry(*env, &key, &value);
		env++;
		if (strcmp(key, "SHLVL") == 0)
			*value += 1;
		lst_addback(&env_list, lst_new(key, value));
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
			if (env_list->value)
				env_array[i] = ft_strjoin(temp, env_list->value);
			else
				env_array[i] = ft_strjoin(temp, "");
			temp = NULL;
			i++;
		}
		env_list = env_list->next;
	}
	env_array[i] = NULL;
	return (env_array);
}
