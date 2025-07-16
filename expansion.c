/* *************************************echo "''"'""'$USER'""'"''"************************************* */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mecavus <mecavus@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 12:58:55 by emrozmen          #+#    #+#             */
/*   Updated: 2025/07/16 13:19:21 by mecavus          ###   ########.fr       */
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

static char	*remove_quotes(const char *value, t_tokentype type)
{
	int		len;
	char	*result;

	if (!value)
		return (ft_strdup(""));
		
	len = ft_strlen((char *)value);
	
	if (type == S_QUOT || type == D_QUOT)
	{
		if (len >= 2 && value[0] == value[len-1] && 
			(value[0] == '\'' || value[0] == '\"'))
		{
			result = ft_substr((char *)value, 1, len - 2);
			return (result);
		}
	}
	return (ft_strdup((char *)value));
}

static int	has_quotes_or_expansion(const char *value)
{
	return (ft_strchr((char *)value, '$') || ft_strchr((char *)value, '\'')
		|| ft_strchr((char *)value, '\"'));
}

static char	*process_mixed_content(const char *value, t_env *env, int *was_quoted)
{
	char	*result;
	int		i;
	int		s_quot;  // Single quote flag
	int		d_quot;  // Double quote flag
	int		found_quotes; // Track if we actually found quotes

	if (!value)
		return (ft_strdup(""));

	result = ft_strdup("");
	i = 0;
	s_quot = 0;  // Initialize to 0
	d_quot = 0;  // Initialize to 0
	found_quotes = 0;  // Track if we encounter any quotes

	while (value[i])
	{
		if (value[i] == '\'')
		{
			found_quotes = 1;  // We found quotes
			if (d_quot)
			{
				// Inside double quotes: preserve the single quote literally
				result = append_char(result, value, &i);
			}
			else
			{
				// Not inside double quotes: toggle single quote mode and skip
				s_quot = !s_quot;
				i++;
			}
		}
		else if (value[i] == '\"')
		{
			found_quotes = 1;  // We found quotes
			if (s_quot)
			{
				// Inside single quotes: preserve the double quote literally
				result = append_char(result, value, &i);
			}
			else
			{
				// Not inside single quotes: toggle double quote mode and skip
				d_quot = !d_quot;
				i++;
			}
		}
		else if (value[i] == '$' && !s_quot)
		{
			// Expand variables only when NOT inside single quotes
			i++;
			if (value[i] && value[i] == '?')
				result = handle_exit_status(result, &i);
			else
				result = append_variable(result, value, &i, env);
		}
		else
		{
			// Regular character - append it
			result = append_char(result, value, &i);
		}
	}
	
	// Set was_quoted based on whether we actually found quotes
	*was_quoted = found_quotes;
	return (result);
}

static char	*expand_assignment(const char *value, t_env *env, int *was_quoted)
{
	char	*eq_pos;
	char	*var_name;
	char	*var_value;
	char	*expanded_value;
	char	*result;

	eq_pos = ft_strchr((char *)value, '=');
	
	// Get variable name (everything before =)
	var_name = ft_substr((char *)value, 0, eq_pos - value);
	
	// Get variable value (everything after =)
	var_value = ft_strdup(eq_pos + 1);
	
	// Expand the value part only
	expanded_value = process_mixed_content(var_value, env, was_quoted);
	
	// Reconstruct the assignment
	result = ft_strjoin(var_name, "=");
	result = ft_strjoin(result, expanded_value);
	
	return (result);
}

static int	is_assignment(const char *value)
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

void	solve_expansion(t_token *list, t_env *env)
{
	char	*expanded_value;
	t_token	*current;
	t_token	*next;
	int		was_quoted;
	char 	*inner_content;

	current = list;
	while (current)
	{
		next = current->next;
		
		// Handle variable assignments specially (don't word split)
		if (current->type == WORD && is_assignment(current->value))
		{
			expanded_value = expand_assignment(current->value, env, &was_quoted);
			current->value = expanded_value;
			current->type = WORD;
		}
		// Handle simple single quotes (pure single quote tokens)
		else if (current->type == S_QUOT)
		{
			expanded_value = remove_quotes(current->value, S_QUOT);
			current->value = expanded_value;
			current->type = WORD;
		}
		// Handle simple double quotes (pure double quote tokens)
		else if (current->type == D_QUOT)
		{
			// For D_QUOT tokens, first remove outer quotes, then process content
			inner_content = remove_quotes(current->value, D_QUOT);
			expanded_value = process_mixed_content(inner_content, env, &was_quoted);
			current->value = expanded_value;
			current->type = WORD;
		}
		// Handle expansion tokens and complex mixed content
		else if (current->type == EXPAND || 
			(current->type == WORD && has_quotes_or_expansion(current->value)))
		{
			expanded_value = process_mixed_content(current->value, env, &was_quoted);
			if (ft_strlen(expanded_value) == 0)
			{
				current->value = expanded_value;
				current->type = WORD;
				current->is_removed = 1;
			}
			else if (was_quoted)
			{
				// If content was quoted, treat as single word (no word splitting)
				current->value = expanded_value;
				current->type = WORD;
			}
			else if (needs_word_splitting(expanded_value))
			{
				// Only split if content wasn't originally quoted
				handle_word_splitting(current, expanded_value);
			}
			else
			{
				current->value = expanded_value;
				current->type = WORD;
			}
		}
		current = next;
	}
}
