/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variable.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mecavus <mecavus@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 12:56:47 by emrozmen          #+#    #+#             */
/*   Updated: 2025/07/22 20:08:07 by mecavus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*safe_strjoin_replace(char *old_str, char *append_str)
{
	char	*new_str;

	new_str = ft_strjoin(old_str, append_str);
	return (new_str);
}

char	*handle_exit_status(char *result, int *i)
{
	char	*tmp;
	char	*new_res;

	tmp = ft_itoa(exit_status(0, PULL));
	new_res = safe_strjoin_replace(result, tmp);
	*i += 1;
	return (new_res);
}

char	*append_char(char *result, const char *value, int *i)
{
	char	*tmp;
	char	*new_res;

	tmp = ft_substr((char *)value, *i, 1);
	new_res = safe_strjoin_replace(result, tmp);
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
		new_res = safe_strjoin_replace(result, val);
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
		new_res = safe_strjoin_replace(result, "$");
		return (new_res);
	}
	var = ft_substr((char *)value, start, *i - start);
	new_res = join_with_env_value(result, var, env);
	return (new_res);
}
