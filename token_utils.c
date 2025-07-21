/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mecavus <mecavus@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 16:16:54 by mecavus           #+#    #+#             */
/*   Updated: 2025/07/21 16:17:29 by mecavus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	has_mixed_quotes(const char *str)
{
	int	has_single;
	int	has_double;
	int	i;

	has_single = 0;
	has_double = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] == '\'')
			has_single = 1;
		else if (str[i] == '\"')
			has_double = 1;
		i++;
	}
	return (has_single && has_double);
}

static int	identify_extended(t_tokentype *type, char *str)
{
	int	len;

	len = ft_strlen(str);
	if (has_mixed_quotes(str))
	{
		*type = EXPAND;
		return (1);
	}
	if (str[0] == '\'' && str[len - 1] == '\'' && len >= 2)
	{
		*type = S_QUOT;
		return (1);
	}
	if (str[0] == '\"' && str[len - 1] == '\"' && len >= 2)
	{
		*type = D_QUOT;
		return (1);
	}
	if (str[0] == '$')
	{
		*type = EXPAND;
		return (1);
	}
	return (0);
}

t_tokentype	identify_token_type(char *str,	t_token *last)
{
	t_tokentype	ret;

	if (!str || !*str)
		return (WORD);
	if (last && last->type == HERDOC)
		return (HERKEY);
	if (last && (last->type == R_IN || last->type == R_OUT
			|| last->type == APPEND))
		return (R_FILE);
	if (ft_strcmp(str, "|") == 0)
		return (PIPE);
	if (ft_strcmp(str, ">>") == 0)
		return (APPEND);
	if (ft_strcmp(str, "<<") == 0)
		return (HERDOC);
	if (ft_strcmp(str, "<") == 0)
		return (R_IN);
	if (ft_strcmp(str, ">") == 0)
		return (R_OUT);
	if (identify_extended(&ret, str))
		return (ret);
	return (WORD);
}
