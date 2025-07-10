/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mecavus <mecavus@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 12:00:00 by mecavus          #+#    #+#             */
/*   Updated: 2025/07/10 12:00:00 by mecavus         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_command	*parse_tokens_to_commands(t_token *tokens)
{
	t_command	*cmd_list;
	t_command	*current_cmd;
	t_token		*current_token;
	int			ac;
	int			i;

	cmd_list = NULL;
	current_token = tokens;
	while (current_token)
	{
		current_cmd = create_command();
		ac = count_word_tokens(current_token);
		if (ac > 0)
		{
			current_cmd->args = ft_malloc(sizeof(char *) * (ac + 1), ALLOC);
			i = 0;
			while (current_token && current_token->type != PIPE)
			{
				if (current_token->type == WORD)
				{
					current_cmd->args[i++] = ft_strdup(current_token->value);
					if (!current_cmd->command)
						current_cmd->command = ft_strdup(current_token->value);
				}
				current_token = current_token->next;
			}
			current_cmd->args[i] = NULL;
		}
		add_command(&cmd_list, current_cmd);
		if (current_token && current_token->type == PIPE)
			current_token = current_token->next;
	}
	return (cmd_list);
}
