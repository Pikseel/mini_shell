/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mecavus <mecavus@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 19:48:44 by mecavus           #+#    #+#             */
/*   Updated: 2025/07/07 19:51:38 by mecavus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ft_strtrim(char *s, char c)
{
	int		start;
	int		end;
	char	*res;
	int		i;

	if (!s)
		return (NULL);
	start = 0;
	while (s[start] && s[start] == c)
		start++;
	end = start;
	while (s[end])
		end++;
	end--;
	while (end >= start && s[end] == c)
		end--;
	res = ft_malloc(end - start + 2, ALLOC);
	if (!res)
		return (NULL);
	i = 0;
	while (start <= end)
		res[i++] = s[start++];
	res[i] = '\0';
	return (res);
}
