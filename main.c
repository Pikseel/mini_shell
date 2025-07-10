/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mecavus <mecavus@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 12:29:43 by emrozmen          #+#    #+#             */
/*   Updated: 2025/07/10 17:12:35 by mecavus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	init_shell(t_main *shell, char **env)
{
	shell->input = NULL;
	shell->tkn_list = NULL;
	shell->cmd_list = NULL;
	exit_status(0, PUSH);
	shell->env_list = init_env_list(env);
}

int	check_exit(char *input)
{
	if (!input)
	{
		ft_putstr_fd("exit\n", 1);
		clear_history();
		rl_clear_history();
		ft_malloc(0, CLEAR);
		exit(exit_status(0, PULL));
		return (1);
	}
	return (0);
}

void	check_ac(int ac)
{
	if (ac > 1)
	{
		ft_putstr_fd("minishell: no arguments please\n", 2);
		exit(127);
	}
	if (!isatty(STDIN_FILENO))
	{
		ft_putstr_fd("use terminal please.\n", 2);
		exit(1);
	}
}

int	main(int ac , char **av, char **env)
{
	t_main	shell;

	(void)av;
	check_ac(ac);
	init_shell(&shell, env);
	while(1)
	{
		init_signal();
		shell.input = readline("minishell> ");
		if (check_exit(shell.input))
			break ;
		if (shell.input)
			add_history(shell.input);
		if (check_syntax(shell.input))
		{
			exit_status(2, PUSH);
			continue ;
		}
		shell.tkn_list = tokenize_input(shell.input);
		if (!shell.tkn_list)
		{
			exit_status(0, PUSH);
			continue ;
		}
		solve_expansion(shell.tkn_list, shell.env_list);
		shell.cmd_list = parse_tokens_to_commands(shell.tkn_list);
		if (shell.cmd_list)
		{
			if (!shell.cmd_list->next)
			{
				if (shell.cmd_list->args && shell.cmd_list->args[0])
					execute_command(shell.cmd_list->args, shell.env_list);
			}
			else
				execute_piped_commands(shell.cmd_list, shell.env_list);
		}
		if (shell.input)
		{
			free(shell.input);
			shell.input = NULL;
		}
		shell.tkn_list = NULL;
		shell.cmd_list = NULL;
		//heredoc, append, redirection, heredoc sinyal, hata durumları, norm kuralları, leak,
	}
}
