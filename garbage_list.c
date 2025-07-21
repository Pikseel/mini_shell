/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbage_list.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mecavus <mecavus@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 16:10:00 by mecavus           #+#    #+#             */
/*   Updated: 2025/07/21 17:44:02 by mecavus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_garbage	*lstlast_garbage(t_garbage *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next)
		lst = lst->next;
	return (lst);
}

void	add_garbage(t_garbage **lst, t_garbage *new)
{
	if (!lst)
		return ;
	if (!*lst)
		*lst = new;
	else
		lstlast_garbage(*lst)->next = new;
}

t_garbage	*new_garbage(void *adress)
{
	t_garbage	*new;

	new = malloc (sizeof(t_garbage));
	if (!new)
	{
		free(adress);
		perror("malloc()");
		ft_malloc(0, CLEAR);
		return (NULL);
	}
	new->address = adress;
	new->next = NULL;
	return (new);
}
