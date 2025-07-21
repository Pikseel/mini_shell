/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_utils2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mecavus <mecavus@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 16:01:18 by mecavus           #+#    #+#             */
/*   Updated: 2025/07/21 18:55:12 by mecavus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*handle_single_quote(char *result, const char *value,
		t_quote_state *state)
{
	state->found_quotes = 1;
	if (state->d_quot)
		result = append_char(result, value, &state->i);
	else
	{
		state->s_quot = !(state->s_quot);
		state->i++;
	}
	return (result);
}

static char	*handle_double_quote(char *result, const char *value,
		t_quote_state *state)
{
	state->found_quotes = 1;
	if (state->s_quot)
		result = append_char(result, value, &state->i);
	else
	{
		state->d_quot = !(state->d_quot);
		state->i++;
	}
	return (result);
}

static char	*handle_dollar_sign(char *result, const char *value,
		t_quote_state *state, t_env *env)
{
	if (state->s_quot)
		result = append_char(result, value, &state->i);
	else
	{
		state->i++;
		if (value[state->i] && value[state->i] == '?')
			result = handle_exit_status(result, &state->i);
		else
			result = append_variable(result, value, &state->i, env);
	}
	return (result);
}

static char	*process_character(char *result, const char *value,
		t_quote_state *state, t_env *env)
{
	if (value[state->i] == '\'')
		result = handle_single_quote(result, value, state);
	else if (value[state->i] == '\"')
		result = handle_double_quote(result, value, state);
	else if (value[state->i] == '$')
		result = handle_dollar_sign(result, value, state, env);
	else
		result = append_char(result, value, &state->i);
	return (result);
}

char	*process_mixed_content(const char *value, t_env *env, int *was_quoted)
{
	char			*result;
	t_quote_state	state;

	if (!value)
		return (ft_strdup(""));
	result = ft_strdup("");
	state.i = 0;
	state.s_quot = 0;
	state.d_quot = 0;
	state.found_quotes = 0;
	while (value[state.i])
		result = process_character(result, value, &state, env);
	*was_quoted = state.found_quotes;
	return (result);
}
