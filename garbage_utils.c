/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbage_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mecavus <mecavus@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 16:00:00 by mecavus           #+#    #+#             */
/*   Updated: 2025/07/21 17:44:02 by mecavus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	clear_it(t_garbage *garbage)
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

static void	clear_command_args(t_command *cmd)
{
	int	i;

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
}

void	clear_command_list(t_main *shell)
{
	t_command	*cmd;
	t_command	*next_cmd;

	if (shell && shell->cmd_list)
	{
		cmd = shell->cmd_list;
		while (cmd)
		{
			next_cmd = cmd->next;
			cmd->command = NULL;
			clear_command_args(cmd);
			cmd->next = NULL;
			cmd = next_cmd;
		}
		shell->cmd_list = NULL;
	}
}
