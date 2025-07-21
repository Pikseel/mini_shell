/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mecavus <mecavus@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 17:13:44 by emrozmen          #+#    #+#             */
/*   Updated: 2025/07/21 16:16:40 by mecavus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	add_token_back(t_token **lst, t_token *new)
{
	t_token	*tmp;

	if (!lst || !new)
		return ;
	if (!*lst)
	{
		*lst = new;
		return ;
	}
	tmp = *lst;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
	new->prev = tmp;
}

static t_token	*fill_token_list(char **arr)
{
	t_token	*list;
	t_token	*new;
	t_token	*last;
	int		i;

	list = NULL;
	last = NULL;
	i = -1;
	while (arr[++i])
	{
		new = ft_malloc(sizeof(t_token), ALLOC);
		new->value = ft_strdup(arr[i]);
		new->type = identify_token_type(arr[i], last);
		new->next = NULL;
		new->prev = NULL;
		new->fd_rdir = -1;
		new->is_removed = 0;
		new->is_invalid_redir = 0;
		add_token_back(&list, new);
		last = new;
	}
	return (list);
}

t_token	*tokenize_input(char *input)
{
	char	**input_arr;
	t_token	*list;

	input_arr = split_with_quotes(input);
	if (!input_arr)
		return (NULL);
	list = fill_token_list(input_arr);
	return (list);
}
