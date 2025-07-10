/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emrozmen <emrozmen@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 12:24:03 by emrozmen          #+#    #+#             */
/*   Updated: 2025/07/07 12:24:54 by emrozmen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ft_strjoin(char *s1, char *s2)
{
	char	*string;
	size_t	len;

	if (s1 == NULL && s2 == NULL)
		return (NULL);
	if (!s1 && s2)
		return (ft_strdup(s2));
	if (s1 && !s2)
		return (ft_strdup(s1));
	len = ft_strlen(s1) + ft_strlen(s2) + 1;
	string = ft_malloc(len, ALLOC);
	ft_strlcpy(string, s1, len);
	ft_strlcat(string, s2, len);
	return (string);
}
