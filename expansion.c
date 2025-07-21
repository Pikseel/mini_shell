/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mecavus <mecavus@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 12:58:55 by emrozmen          #+#    #+#             */
/*   Updated: 2025/07/21 15:43:34 by mecavus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_env_value(t_env *env, const char *varname)
{
	while (env)
	{
		if (ft_strcmp(env->key, varname) == 0)
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

void	solve_expansion(t_token *list, t_env *env)
{
	t_token	*current;
	t_token	*next;

	current = list;
	while (current)
	{
		next = current->next;
		process_token_expansion(current, env);
		current = next;
	}
}
