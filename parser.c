/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mecavus <mecavus@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 12:00:00 by mecavus           #+#    #+#             */
/*   Updated: 2025/07/21 19:16:06 by mecavus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_command	*process_single_command(t_token *current_tkn,
				t_env *env_list)
{
	t_command	*cmd;
	t_token		*start_tkn;
	int			ac;

	cmd = create_command();
	start_tkn = current_tkn;
	handle_redirections(current_tkn, cmd, env_list);
	ac = count_word_tokens(start_tkn);
	if (ac > 0)
		populate_command_args(cmd, start_tkn, ac);
	return (cmd);
}

t_command	*parse_tkn_to_cmds(t_token *tokens, t_env *env_list)
{
	t_command	*cmd_list;
	t_command	*current_cmd;
	t_token		*current_tkn;

	cmd_list = NULL;
	current_tkn = tokens;
	while (current_tkn)
	{
		current_cmd = process_single_command(current_tkn, env_list);
		while (current_tkn && current_tkn->type != PIPE)
			current_tkn = current_tkn->next;
		add_command(&cmd_list, current_cmd);
		if (current_tkn && current_tkn->type == PIPE)
			current_tkn = current_tkn->next;
	}
	return (cmd_list);
}
