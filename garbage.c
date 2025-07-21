/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbage.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mecavus <mecavus@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 14:53:17 by emrozmen          #+#    #+#             */
/*   Updated: 2025/07/21 17:44:02 by mecavus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	clear_token_list(t_main *shell)
{
	t_token	*token;
	t_token	*next_token;

	if (shell && shell->tkn_list)
	{
		token = shell->tkn_list;
		while (token)
		{
			next_token = token->next;
			token->value = NULL;
			token->next = NULL;
			token->prev = NULL;
			token = next_token;
		}
		shell->tkn_list = NULL;
	}
}

void	clear_pointers(t_main *shell)
{
	clear_command_list(shell);
	clear_token_list(shell);
	if (shell)
	{
		shell->input = NULL;
		shell->env_list = NULL;
	}
}

static void	*handle_malloc_flags(size_t size, int flag,
			t_garbage **garbage, t_main **main_shell)
{
	if (flag == CLEAR)
	{
		clear_it(*garbage);
		*garbage = NULL;
		return (NULL);
	}
	else if (flag == SET_SHELL)
	{
		*main_shell = (t_main *) size;
		return (NULL);
	}
	else if (flag == GET_SHELL)
		return (*main_shell);
	return ((void *)1);
}

void	*ft_malloc(size_t size, int flag)
{
	static t_garbage	*garbage;
	static t_main		*main_shell;
	void				*ret;
	void				*flag_result;

	flag_result = handle_malloc_flags(size, flag, &garbage, &main_shell);
	if (flag_result != (void *)1)
		return (flag_result);
	ret = malloc(size);
	if (!ret)
	{
		clear_it(garbage);
		garbage = NULL;
		perror("malloc()");
		clear_exit(main_shell, 1, NULL);
	}
	add_garbage(&garbage, new_garbage(ret));
	return (ret);
}
