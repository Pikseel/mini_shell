/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_quotes.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mecavus <mecavus@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 12:47:41 by emrozmen          #+#    #+#             */
/*   Updated: 2025/07/22 20:16:07 by mecavus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	skip_word(const char **s)
{
	int	len;
	int	op_len;

	op_len = is_shell_operator(*s, 0);
	if (op_len > 0)
		*s += op_len;
	else
	{
		len = word_len(*s);
		*s += len;
	}
}

void	add_word_to_result(char **result, int *i, const char **s)
{
	int	len;
	int	op_len;

	op_len = is_shell_operator(*s, 0);
	if (op_len > 0)
	{
		result[(*i)++] = extract_word(*s, op_len);
		*s += op_len;
	}
	else
	{
		len = word_len(*s);
		result[(*i)++] = extract_word(*s, len);
		*s += len;
	}
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
			if (!*s)
				break ;
			skip_word(&s);
		}
	}
	return (count);
}

static char	**fill_result_array(const char *s, int word_count)
{
	char	**result;
	int		i;

	i = 0;
	result = ft_malloc(sizeof(char *) * (word_count + 1), ALLOC);
	while (*s)
	{
		while (ft_is_space(*s))
			s++;
		if (*s)
			add_word_to_result(result, &i, &s);
	}
	result[i] = NULL;
	return (result);
}

char	**split_with_quotes(const char *s)
{
	int		word_count;

	word_count = count_words(s);
	return (fill_result_array(s, word_count));
}
