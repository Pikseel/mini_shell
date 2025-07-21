/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mecavus <mecavus@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 12:21:55 by mecavus           #+#    #+#             */
/*   Updated: 2025/07/21 16:10:10 by mecavus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*remove_quotes(const char *value, t_tokentype type)
{
	int		len;
	char	*result;

	if (!value)
		return (ft_strdup(""));
	len = ft_strlen((char *)value);
	if (type == S_QUOT || type == D_QUOT)
	{
		if (len >= 2 && value[0] == value[len - 1]
			&& (value[0] == '\'' || value[0] == '\"'))
		{
			result = ft_substr((char *)value, 1, len - 2);
			return (result);
		}
	}
	return (ft_strdup((char *)value));
}

char	*expand_assignment(const char *value, t_env *env, int *was_quoted)
{
	char	*eq_pos;
	char	*var_name;
	char	*var_value;
	char	*expanded_value;
	char	*result;

	eq_pos = ft_strchr((char *)value, '=');
	var_name = ft_substr((char *)value, 0, eq_pos - value);
	var_value = ft_strdup(eq_pos + 1);
	expanded_value = process_mixed_content(var_value, env, was_quoted);
	result = ft_strjoin(var_name, "=");
	result = ft_strjoin(result, expanded_value);
	return (result);
}

int	is_assignment(const char *value)
{
	char	*eq_pos;
	int		i;

	if (!value)
		return (0);
	eq_pos = ft_strchr((char *)value, '=');
	if (!eq_pos)
		return (0);
	i = 0;
	while (value + i < eq_pos)
	{
		if (i == 0 && !ft_isalpha(value[i]) && value[i] != '_')
			return (0);
		if (!ft_isalnum(value[i]) && value[i] != '_')
			return (0);
		i++;
	}
	return (1);
}
