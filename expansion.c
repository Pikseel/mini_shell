/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mecavus <mecavus@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 12:58:55 by emrozmen          #+#    #+#             */
/*   Updated: 2025/07/16 11:58:01 by mecavus          ###   ########.fr       */
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

static int	check_type(t_token tkn)
{
	if (tkn.type == EXPAND || tkn.type == D_QUOT || tkn.type == S_QUOT)
		return (1);
	if (tkn.type == WORD && (ft_strchr(tkn.value, '$')
		|| ft_strchr(tkn.value, '\'') || ft_strchr(tkn.value, '\"')))
		return (1);
	return (0);
}

static char	*process_mixed_quotes(const char *value, t_env *env)
{
	char	*result;
	int		i;
	int		in_single;
	int		in_double;

	if (!value)
		return (ft_strdup(""));

	result = ft_strdup("");
	i = 0;
	in_single = 0;
	in_double = 0;
	while (value[i])
	{
		if (value[i] == '\'' && !in_double)
		{
			in_single = !in_single;
			i++;
		}
		else if (value[i] == '\"' && !in_single)
		{
			in_double = !in_double;
			i++;
		}
		else if (value[i] == '$' && !in_single)
		{
			i++;
			if (value[i] == '?')
				result = handle_exit_status(result, &i);
			else
				result = append_variable(result, value, &i, env);
		}
		else
			result = append_char(result, value, &i);
	}
	return (result);
}

void	solve_expansion(t_token *list, t_env *env)
{
	char	*expanded_value;
	t_token	*current;
	t_token	*next;

	current = list;
	while (current)
	{
		next = current->next;
		if (check_type(*current))
		{
			expanded_value = process_mixed_quotes(current->value, env);
			if (ft_strlen(expanded_value) == 0)
			{
				current->value = expanded_value;
				current->type = WORD;
				if (expanded_value[0] == '\0')
					current->is_removed = 1;		
			}
			else if ((current->type == WORD || current->type == EXPAND)
				&& needs_word_splitting(expanded_value))
				handle_word_splitting(current, expanded_value);
		}
		current = next;
	}
}
