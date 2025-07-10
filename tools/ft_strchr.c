/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emrozmen <emrozmen@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 12:28:03 by emrozmen          #+#    #+#             */
/*   Updated: 2025/07/07 12:28:05 by emrozmen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ft_strchr( char *s, int c)
{
	if (!s)
		return (NULL);
	while (*s && (*s != c))
		s++;
	if (*s == c)
		return (s);
	return (NULL);
}
