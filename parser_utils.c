/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mecavus <mecavus@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 15:45:00 by mecavus           #+#    #+#             */
/*   Updated: 2025/07/21 19:00:20 by mecavus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_word_tokens(t_token *tokens)
{
	int		count;
	t_token	*current;

	count = 0;
	current = tokens;
	while (current && current->type != PIPE)
	{
		if (current->type == WORD && current->is_removed == 0)
			count++;
		current = current->next;
	}
	return (count);
}

t_command	*create_command(void)
{
	t_command	*cmd;

	cmd = ft_malloc(sizeof(t_command), ALLOC);
	cmd->command = NULL;
	cmd->args = NULL;
	cmd->input_fd = STDIN_FILENO;
	cmd->output_fd = STDOUT_FILENO;
	cmd->redirect_failed = 0;
	cmd->next = NULL;
	return (cmd);
}

void	add_command(t_command **cmd_list, t_command *new_cmd)
{
	t_command	*current;

	if (!*cmd_list)
	{
		*cmd_list = new_cmd;
		return ;
	}
	current = *cmd_list;
	while (current->next)
		current = current->next;
	current->next = new_cmd;
}

void	populate_command_args(t_command *cmd, t_token *start_tkn, int ac)
{
	t_token	*current_tkn;
	int		i;

	cmd->args = ft_malloc(sizeof(char *) * (ac + 1), ALLOC);
	i = 0;
	current_tkn = start_tkn;
	while (current_tkn && current_tkn->type != PIPE)
	{
		if (current_tkn->type == WORD && current_tkn->is_removed == 0
			&& i < ac)
		{
			cmd->args[i++] = ft_strdup(current_tkn->value);
			if (!cmd->command)
				cmd->command = ft_strdup(current_tkn->value);
		}
		current_tkn = current_tkn->next;
	}
	cmd->args[i] = NULL;
}
