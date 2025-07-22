/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_quotes_utils2.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mecavus <mecavus@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 20:45:00 by mecavus           #+#    #+#             */
/*   Updated: 2025/07/22 20:16:47 by mecavus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	handle_quote_in_word(const char *s, int len, char *quote)
{
	int	result;

	result = handle_quote(s, len, quote, END);
	if (result == -1)
	{
		return (-1);
	}
	return (result);
}

static int	handle_operator_in_word(const char *s, int len)
{
	int	op_len;

	op_len = is_shell_operator(s, len);
	if (op_len > 0)
	{
		if (len == 0)
			len += op_len;
		return (-1);
	}
	return (len + 1);
}

static int	process_word_char(const char *s, int len, char *quote)
{
	int	result;

	if (!*quote && (s[len] == '\'' || s[len] == '\"'))
		return (handle_quote(s, len, quote, START));
	else if (*quote && s[len] == *quote)
	{
		result = handle_quote_in_word(s, len, quote);
		if (result == -1)
			return (-1);
		return (result);
	}
	else if (!*quote && ft_is_space(s[len]))
		return (-2);
	else if (!*quote)
	{
		result = handle_operator_in_word(s, len);
		if (result == -1)
			return (-2);
		return (result);
	}
	else
		return (len + 1);
}

int	word_len(const char *s)
{
	int		len;
	char	quote;
	int		result;

	len = 0;
	quote = 0;
	while (s[len])
	{
		result = process_word_char(s, len, &quote);
		if (result == -1)
		{
			len++;
			break ;
		}
		if (result == -2)
			break ;
		len = result;
	}
	return (len);
}

int	handle_quote(const char *s, int len, char *quote, int flag)
{
	if (flag == START)
		return (handle_quote_start(s, len, quote));
	else
		return (handle_quote_end(s, len, quote));
}
