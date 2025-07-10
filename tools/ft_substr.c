/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emrozmen <emrozmen@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 12:27:45 by emrozmen          #+#    #+#             */
/*   Updated: 2025/07/07 12:33:32 by emrozmen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ft_substr(char *s, unsigned int start, size_t len)
{
	char	*str;
	size_t	tot_len;

	if (s == NULL)
		return (NULL);
	if (start > ft_strlen(s) || !len)
		return (NULL);
	if (len > ft_strlen(s + start))
		tot_len = ft_strlen(s + start) + 1;
	else
		tot_len = len + 1;
	str = ft_malloc(tot_len, ALLOC);
	ft_strlcpy(str, s + start, tot_len);
	return (str);
}
