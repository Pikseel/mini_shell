/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_quotes_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mecavus <mecavus@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 20:30:00 by mecavus           #+#    #+#             */
/*   Updated: 2025/07/21 20:33:38 by mecavus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_shell_operator(const char *s, int pos)
{
	if (s[pos] == '|')
		return (1);
	if (s[pos] == '<')
	{
		if (s[pos + 1] == '<')
			return (2);
		return (1);
	}
	if (s[pos] == '>')
	{
		if (s[pos + 1] == '>')
			return (2);
		return (1);
	}
	return (0);
}

int	handle_quote_start(const char *s, int len, char *quote)
{
	*quote = s[len];
	return (len + 1);
}

int	handle_quote_end(const char *s, int len, char *quote)
{
	*quote = 0;
	len++;
	if (s[len] && !ft_is_space(s[len]) && !is_shell_operator(s, len))
		return (len);
	else
		return (-1);
}

char	*extract_word(const char *s, int len)
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
