/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbage.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emrozmen <emrozmen@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 14:53:17 by emrozmen          #+#    #+#             */
/*   Updated: 2025/07/04 15:05:15 by emrozmen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_garbage	*lstlast_garbage(t_garbage *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next)
		lst = lst->next;
	return (lst);
}

static void	add_garabage(t_garbage **lst, t_garbage *new)
{
	if (!lst)
		return ;
	if (!*lst)
		*lst = new;
	else
		lstlast_garbage(*lst)->next = new;
}

static t_garbage	*new_garbage(void *adress)
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

static void	clear_it(t_garbage *garbage)
{
	t_garbage	*clear;

	while (garbage)
	{
		clear = garbage->next;
		free(garbage->address);
		free(garbage);
		garbage = clear;
	}
}

void	*ft_malloc(size_t size, int flag)
{
	static t_garbage	*garbage;
	void				*ret;

	if (flag == CLEAR)
	{
		clear_it(garbage);
		return (NULL);
	}
	ret = malloc(size);
	if (!ret)
	{
		clear_it(garbage);
		perror("malloc()");
		exit(1);
	}
	add_garabage(&garbage, new_garbage(ret));
	return (ret);
}
