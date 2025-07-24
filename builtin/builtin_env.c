/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mecavus <mecavus@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 12:00:00 by mecavus           #+#    #+#             */
/*   Updated: 2025/07/24 09:32:04 by mecavus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	builtin_env(t_env *env_list)
{
	while (env_list)
	{
		if (env_list->value)
		{
			ft_putstr_fd(env_list->key, 1);
			ft_putstr_fd("=", 1);
			if (ft_strcmp(env_list->key, "_") == 0)
				ft_putstr_fd("/usr/bin/env", 1);
			else
				ft_putstr_fd(env_list->value, 1);
			ft_putstr_fd("\n", 1);
		}
		env_list = env_list->next;
	}
}
