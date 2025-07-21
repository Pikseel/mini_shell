/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_tokens.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mecavus <mecavus@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 12:21:55 by mecavus           #+#    #+#             */
/*   Updated: 2025/07/21 15:43:34 by mecavus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_single_quote_token(t_token *current)
{
	char	*expanded_val;

	expanded_val = remove_quotes(current->value, S_QUOT);
	current->value = expanded_val;
	current->type = WORD;
}

static void	handle_double_quote_token(t_token *current, t_env *env)
{
	char	*expanded_val;
	char	*inner_data;
	int		was_quoted;

	inner_data = remove_quotes(current->value, D_QUOT);
	expanded_val = process_mixed_content(inner_data, env, &was_quoted);
	current->value = expanded_val;
	current->type = WORD;
}

static int	needs_expansion(t_token *current)
{
	if (current->type == EXPAND)
		return (1);
	if (current->type == WORD && (ft_strchr(current->value, '$')
			|| ft_strchr(current->value, '\'')
			|| ft_strchr(current->value, '\"')))
		return (1);
	return (0);
}

static void	handle_expansion_token(t_token *current, t_env *env)
{
	char	*expanded_val;
	int		was_quoted;

	expanded_val = process_mixed_content(current->value, env, &was_quoted);
	if (ft_strlen(expanded_val) == 0)
	{
		current->value = expanded_val;
		current->type = WORD;
		current->is_removed = 1;
	}
	else if (was_quoted)
	{
		current->value = expanded_val;
		current->type = WORD;
	}
	else if (needs_word_splitting(expanded_val))
		handle_word_splitting(current, expanded_val);
	else
	{
		current->value = expanded_val;
		current->type = WORD;
	}
}

void	process_token_expansion(t_token *current, t_env *env)
{
	char	*expanded_val;
	int		was_quoted;

	if (current->type == WORD && is_assignment(current->value))
	{
		expanded_val = expand_assignment(current->value, env, &was_quoted);
		current->value = expanded_val;
		current->type = WORD;
	}
	else if (current->type == S_QUOT)
		handle_single_quote_token(current);
	else if (current->type == D_QUOT)
		handle_double_quote_token(current, env);
	else if (needs_expansion(current))
		handle_expansion_token(current, env);
}
