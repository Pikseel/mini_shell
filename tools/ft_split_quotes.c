/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_quotes.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mecavus <mecavus@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 12:47:41 by emrozmen          #+#    #+#             */
/*   Updated: 2025/07/16 15:06:02 by mecavus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	is_whitespace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

static int	word_len(const char *s)
{
	int		len = 0;
	char	quote = 0;

	while (s[len])
	{
		if (!quote && (s[len] == '\'' || s[len] == '\"'))
		{
			quote = s[len];
			len++;
		}
		else if (quote && s[len] == quote)
		{
			quote = 0;
			len++;
			// Check if we should continue concatenating
			// Continue only if the next char is another quote or non-whitespace
			if (s[len] && !is_whitespace(s[len]))
				continue;
			else
				break;
		}
		else if (!quote && is_whitespace(s[len]))
			break;
		else
			len++;
	}
	return (len);
}

static int	count_words(const char *s)
{
	int count = 0;

	while (*s)
	{
		while (is_whitespace(*s))
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
	char	*word = ft_malloc(len + 1, ALLOC);
	int		i = 0;

	// Keep quotes intact - don't remove them here
	// The expansion phase will handle quote removal
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
	int		i = 0;
	int		word_count = count_words(s);
	char	**result = ft_malloc(sizeof(char *) * (word_count + 1), ALLOC);

	while (*s)
	{
		while (is_whitespace(*s))
			s++;
		if (*s)
		{
			int len = word_len(s);
			result[i++] = extract_word(s, len);
			s += len;
		}
	}
	result[i] = NULL;
	return (result);
}
