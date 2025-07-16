/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emrozmen <emrozmen@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 17:13:44 by emrozmen          #+#    #+#             */
/*   Updated: 2025/07/15 12:49:02 by emrozmen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_tokentype	identify_token_type(char *str,	t_token *last)
{
	if (!str || !*str)
		return (WORD);
	if (last && last->type == HERDOC)
		return (HERKEY);
	if (last && (last->type == R_IN || last->type == R_OUT
			|| last->type == APPEND))
		return (R_FILE);
	if (ft_strcmp(str, "|") == 0)
		return (PIPE);
	if (ft_strcmp(str, ">>") == 0)
		return (APPEND);
	if (ft_strcmp(str, "<<") == 0)
		return (HERDOC);
	if (ft_strcmp(str, "<") == 0)
		return (R_IN);
	if (ft_strcmp(str, ">") == 0)
		return (R_OUT);
	if (str[0] == '$')
		return (EXPAND);
	if (str[0] == '\"')
		return (D_QUOT);
	if (str[0] == '\'')
		return (S_QUOT);
	return (WORD);
}

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
