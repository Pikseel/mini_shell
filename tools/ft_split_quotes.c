/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_quotes.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mecavus <mecavus@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 12:47:41 by emrozmen          #+#    #+#             */
/*   Updated: 2025/07/21 15:43:34 by mecavus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	handle_quote(const char *s, int len, char *quote, int flag)
{
	if (flag == START)
	{
		*quote = s[len];
		return (len + 1);
	}
	else
	{
		*quote = 0;
		len++;
		if (s[len] && !ft_is_space(s[len]))
			return (len);
		else
			return (-1);
	}
}

static int	word_len(const char *s)
{
	int		len;
	char	quote;
	int		result;

	len = 0;
	quote = 0;
	while (s[len])
	{
		if (!quote && (s[len] == '\'' || s[len] == '\"'))
			len = handle_quote(s, len, &quote, START);
		else if (quote && s[len] == quote)
		{
			result = handle_quote(s, len, &quote, END);
			if (result == -1)
				break ;
			len = result;
		}
		else if (!quote && ft_is_space(s[len]))
			break ;
		else
			len++;
	}
	return (len);
}

static int	count_words(const char *s)
{
	int	count;

	count = 0;
	while (*s)
	{
		while (ft_is_space(*s))
			s++;
		if (*s)
		{
			count++;
			s += word_len(s);
		}
	}
	return (count);
}

static char	*extract_word(const char *s, int len)
{
	char	*word;
	int		i;

	i = 0;
	word = ft_malloc(len + 1, ALLOC);
	while (i < len)
	{
		word[i] = s[i];
		i++;
	}
	word[i] = '\0';
	return (word);
}

char	**split_with_quotes(const char *s)
{
	int		i;
	int		word_count;
	char	**result;
	int		len;

	i = 0;
	word_count = count_words(s);
	result = ft_malloc(sizeof(char *) * (word_count + 1), ALLOC);
	while (*s)
	{
		while (ft_is_space(*s))
			s++;
		if (*s)
		{
			len = word_len(s);
			result[i++] = extract_word(s, len);
			s += len;
		}
	}
	result[i] = NULL;
	return (result);
}
