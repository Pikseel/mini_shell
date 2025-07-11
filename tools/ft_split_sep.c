/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_sep.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mecavus <mecavus@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 12:00:00 by mecavus          #+#    #+#             */
/*   Updated: 2025/07/10 12:00:00 by mecavus         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	ft_count_words(const char *str, char c)
{
	int	count;
	int	i;

	i = 0;
	count = 0;
	while (str[i])
	{
		while (str[i] && str[i] == c)
			i++;
		if (str[i])
			count++;
		while (str[i] && str[i] != c)
			i++;
	}
	return (count);
}

static char	*ft_word_dup(const char *str, char c)
{
	char	*word;
	int		i;
	int		len;

	len = 0;
	while (str[len] && str[len] != c)
		len++;
	word = ft_malloc(sizeof(char) * (len + 1), ALLOC);
	i = 0;
	while (i < len)
	{
		word[i] = str[i];
		i++;
	}
	word[i] = '\0';
	return (word);
}

char	**ft_split_sep(char *s, char c)
{
	char	**result;
	int		count;
	int		i;

	if (!s)
		return (NULL);
	count = ft_count_words(s, c);
	result = ft_malloc(sizeof(char *) * (count + 1), ALLOC);
	if (!result)
		return (NULL);
	i = 0;
	while (i < count)
	{
		while (*s && *s == c)
			s++;
		result[i] = ft_word_dup(s, c);
		while (*s && *s != c)
			s++;
		i++;
	}
	result[i] = NULL;
	return (result);
}
