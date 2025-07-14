/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variable.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mecavus <mecavus@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 12:56:47 by emrozmen          #+#    #+#             */
/*   Updated: 2025/07/14 17:06:13 by mecavus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*handle_exit_status(char *result, int *i)
{
	char	*tmp;
	char	*new_res;

	tmp = ft_itoa(exit_status(0, PULL));
	new_res = ft_strjoin(result, tmp);
	tmp = NULL;
	*i += 1;
	return (new_res);
}

char	*append_char(char *result, const char *value, int *i)
{
	char	*tmp;
	char	*new_res;

	tmp = ft_substr((char *)value, *i, 1);
	new_res = ft_strjoin(result, tmp);
	tmp = NULL;
	*i += 1;
	return (new_res);
}

static char	*join_with_env_value(char *result, char *var, t_env *env)
{
	char	*val;
	char	*new_res;

	val = get_env_value(env, var);
	if (val)
	{
		new_res = ft_strjoin(result, val);
		return (new_res);
	}
	return (result);
}

char	*append_variable(char *result, const char *value, int *i,
			t_env *env)
{
	int		start;
	char	*var;
	char	*new_res;

	start = *i;
	while (ft_isalnum(value[*i]) || value[*i] == '_')
		(*i)++;
	if (*i == start)
	{
		new_res = ft_strjoin(result, "$");
		return (new_res);
	}
	var = ft_substr((char *)value, start, *i - start);
	new_res = join_with_env_value(result, var, env);
	var = NULL;
	return (new_res);
}
