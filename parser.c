/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mecavus <mecavus@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 12:00:00 by mecavus           #+#    #+#             */
/*   Updated: 2025/07/24 10:24:41 by mecavus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	update_underscore_env(t_env *env_list, char *last_arg)
{
	t_env	*current;

	if (!last_arg)
		return ;
	current = env_list;
	while (current)
	{
		if (ft_strcmp(current->key, "_") == 0)
		{
			current->value = ft_strdup(last_arg);
			return ;
		}
		current = current->next;
	}
	lst_addback(&env_list, lst_new(ft_strdup("_"), ft_strdup(last_arg)));
}

static void	find_and_update_underscore(t_command *cmd_list, t_env *env_list)
{
	char	*last_arg;
	int		arg_count;

	if (!cmd_list || !cmd_list->args)
		return ;
	arg_count = 0;
	while (cmd_list->args[arg_count])
		arg_count++;
	if (arg_count > 0)
	{
		last_arg = cmd_list->args[arg_count - 1];
		update_underscore_env(env_list, last_arg);
	}
}

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
	int			has_pipes;

	cmd_list = NULL;
	current_tkn = tokens;
	has_pipes = 0;
	while (current_tkn)
	{
		if (current_tkn->type == PIPE)
			has_pipes = 1;
		current_tkn = current_tkn->next;
	}
	while (tokens)
	{
		current_cmd = process_single_command(tokens, env_list);
		tokens = skip_to_next_pipe(tokens);
		add_command(&cmd_list, current_cmd);
		if (tokens && tokens->type == PIPE)
			tokens = tokens->next;
	}
	if (!has_pipes)
		find_and_update_underscore(cmd_list, env_list);
	return (cmd_list);
}
