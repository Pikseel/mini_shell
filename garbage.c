/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbage.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mecavus <mecavus@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 14:53:17 by emrozmen          #+#    #+#             */
/*   Updated: 2025/07/18 15:40:14 by mecavus          ###   ########.fr       */
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

static void	add_garbage(t_garbage **lst, t_garbage *new)
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

void	clear_pointers(t_main *shell)
{
	t_command	*cmd;
	t_command	*next_cmd;
	t_token		*token;
	t_token		*next_token;
	int			i;

	if (shell && shell->cmd_list)
	{
		cmd = shell->cmd_list;
		while (cmd)
		{
			next_cmd = cmd->next;
			cmd->command = NULL;
			if (cmd->args)
			{
				i = 0;
				while (cmd->args[i])
				{
					cmd->args[i] = NULL;
					i++;
				}
				cmd->args = NULL;
			}
			cmd->next = NULL;
			cmd = next_cmd;
		}
		shell->cmd_list = NULL;
	}
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
	if (shell)
	{
		shell->input = NULL;
		shell->env_list = NULL;
	}
}

void	*ft_malloc(size_t size, int flag)
{
	static t_garbage	*garbage;
	static t_main		*main_shell;
	void				*ret;

	if (flag == CLEAR)
	{
		clear_it(garbage);
		garbage = NULL;
		return (NULL);
	}
	else if (flag == SET_SHELL)
	{
		main_shell = (t_main *) size;
		return (NULL);
	}
	else if (flag == GET_SHELL)
	{
		return (main_shell);
	}
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
