/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mecavus <mecavus@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 14:26:24 by emrozmen          #+#    #+#             */
/*   Updated: 2025/07/21 19:05:07 by mecavus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	acttual_len(char *s, int i)
{
	int		start;
	char	c;

	start = i;
	while (s[i])
	{
		if (ft_strchr("|<>", s[i]))
			return (i - start);
		else if (s[i] == '\'' || s[i] == '\"')
		{
			c = s[i];
			i++;
			while (s[i] && s[i] != c)
				i++;
			if (!s[i])
				return (i - start);
		}
		else if (ft_is_space(s[i]))
			break ;
		i++;
	}
	return (i - start);
}

static int	get_word_len(char *s, int i)
{
	int	start;

	start = i;
	if (s[i] == '|')
		return (1);
	if (ft_strchr("<>", s[i]))
	{
		while (ft_strchr("<>", s[i]) && s[i])
			i++;
		return (i - start);
	}
	return (acttual_len(s, i));
}

static int	ft_countword(char *s)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	if (!s)
		return (0);
	while (s[i])
	{
		i = skip_spaces(s, i);
		if (!s[i])
			break ;
		i += get_word_len(s, i);
		count++;
	}
	return (count);
}

char	**ft_split(char *s)
{
	int		i;
	int		j;
	int		word_len;
	int		words;
	char	**lst;

	(1) && (i = 0, j = 0, words = ft_countword(s));
	if (words == 0)
		return (NULL);
	lst = ft_malloc(sizeof(char *) * (words + 1), ALLOC);
	while (s[i])
	{
		i = skip_spaces(s, i);
		if (!s[i])
			break ;
		word_len = get_word_len(s, i);
		lst[j++] = ft_substr(s, i, word_len);
		i += word_len;
	}
	lst[j] = NULL;
	return (lst);
}
