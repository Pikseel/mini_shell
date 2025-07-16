/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mecavus <mecavus@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 12:00:00 by mecavus           #+#    #+#             */
/*   Updated: 2025/07/16 12:00:52 by mecavus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	count_word_tokens(t_token *tokens)
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

static t_command	*create_command(void)
{
	t_command	*cmd;

	cmd = ft_malloc(sizeof(t_command), ALLOC);
	cmd->command = NULL;
	cmd->args = NULL;
	cmd->input_fd = STDIN_FILENO;
	cmd->output_fd = STDOUT_FILENO;
	cmd->next = NULL;
	return (cmd);
}

static void	add_command(t_command **cmd_list, t_command *new_cmd)
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

t_command	*parse_tokens_to_commands(t_token *tokens)
{
	t_command	*cmd_list;
	t_command	*current_cmd;
	t_token		*current_tkn;
	int			ac;
	int			i;

	cmd_list = NULL;
	current_tkn = tokens;
	while (current_tkn)
	{
		current_cmd = create_command();
		ac = count_word_tokens(current_tkn);
		if (ac > 0)
		{
			current_cmd->args = ft_malloc(sizeof(char *) * (ac + 1), ALLOC);
			i = 0;
			while (current_tkn && current_tkn->type != PIPE)
			{
				if (current_tkn->type == WORD && current_tkn->is_removed == 0 && i < ac)
				{
					current_cmd->args[i++] = ft_strdup(current_tkn->value);
					if (!current_cmd->command)
						current_cmd->command = ft_strdup(current_tkn->value);
				}
				current_tkn = current_tkn->next;
			}
			current_cmd->args[i] = NULL;
		}
		add_command(&cmd_list, current_cmd);
		if (current_tkn && current_tkn->type == PIPE)
			current_tkn = current_tkn->next;
	}
	return (cmd_list);
}
