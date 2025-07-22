/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emrozmen <emrozmen@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 12:00:00 by mecavus           #+#    #+#             */
/*   Updated: 2025/07/15 12:40:19 by emrozmen         ###   ########.fr       */
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
			ft_putstr_fd(env_list->value, 1);
			ft_putstr_fd("\n", 1);
		}
		env_list = env_list->next;
	}
}
