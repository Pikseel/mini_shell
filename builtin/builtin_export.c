/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mecavus <mecavus@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 13:00:00 by mecavus           #+#    #+#             */
/*   Updated: 2025/07/16 17:46:37 by mecavus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static	void	print_export_format(t_env *env)
{
	while (env)
	{
		ft_putstr_fd("declare -x ", 1);
		ft_putstr_fd(env->key, 1);
		if (env->value)
		{
			ft_putstr_fd("=\"", 1);
			ft_putstr_fd(env->value, 1);
			ft_putstr_fd("\"\n", 1);
		}
		else
			ft_putstr_fd("\n", 1);
		env = env->next;
	}
}

static int	is_valid_identifier(char *str)
{
	int	i;

	if (!str || !*str)
		return (0);
	if (!ft_isalpha(str[0]) && str[0] != '_')
		return (0);
	i = 1;
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

static int	update_existing_env(t_env **env_list, char *key, char *value)
{
	t_env	*current;
	char	*new_value;

	current = *env_list;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			if (value)
			{
				new_value = ft_malloc(ft_strlen(value) + 1, ALLOC);
				ft_strlcpy(new_value, value, ft_strlen(value) + 1);
				current->value = new_value;
			}
			return (1);
		}
		current = current->next;
	}
	return (0);
}

static void	parse_env_arg(char *arg, char **key, char **value)
{
	char	*equal_pos;

	equal_pos = ft_strchr(arg, '=');
	if (!equal_pos)
	{
		*key = ft_malloc(ft_strlen(arg) + 1, ALLOC);
		ft_strlcpy(*key, arg, ft_strlen(arg) + 1);
		*value = NULL;
		return ;
	}
	*key = ft_malloc(equal_pos - arg + 1, ALLOC);
	ft_strlcpy(*key, arg, equal_pos - arg + 1);
	(*key)[equal_pos - arg] = '\0';
	*value = ft_malloc(ft_strlen(equal_pos + 1) + 1, ALLOC);
	ft_strlcpy(*value, equal_pos + 1, ft_strlen(equal_pos + 1) + 1);
}

void	builtin_export(char **args, t_env **env_list)
{
	int		i;
	char	*key;
	char	*value;

	if (!args[1])
	{
		print_export_format(*env_list);
		return ;
	}
	i = 1;
	while (args[i])
	{
		parse_env_arg(args[i], &key, &value);
		if (is_valid_identifier(key))
		{
			if (!update_existing_env(env_list, key, value))
			{
				lst_addback(env_list, lst_new(key, value));
			}
		}
		else
		{
			ft_putstr_fd("minishell: export: `", 2);
			ft_putstr_fd(args[i], 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
			exit_status(1, PUSH);
		}
		i++;
	}
	exit_status(0, PUSH);
}
