/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mecavus <mecavus@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 12:29:43 by emrozmen          #+#    #+#             */
/*   Updated: 2025/07/23 18:08:08 by mecavus          ###   ########.fr       */
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

void	clear_exit(t_main *shell, int exit_code, char *message)
{
	if (message)
	{
		if (exit_code != 0)
			ft_putstr_fd("minishell: ", 2);
		if (exit_code != 0)
			ft_putstr_fd(message, 2);
		else
			ft_putstr_fd(message, 1);
		if (exit_code != 0)
			ft_putstr_fd("\n", 2);
		else
			ft_putstr_fd("\n", 1);
	}
	clear_history();
	rl_clear_history();
	if (shell)
		clear_pointers(shell);
	ft_malloc(0, CLEAR);
	exit(exit_code);
}

static void	check_exit(char *input, t_main *shell)
{
	if (!input)
	{
		ft_putstr_fd("exit\n", 1);
		clear_exit(shell, exit_status(0, PULL), NULL);
	}
	return ;
}

static void	check_ac(int ac)
{
	return ;
	if (ac > 1)
		clear_exit(NULL, 127, "no arguments please");
	if (!isatty(STDIN_FILENO))
		clear_exit(NULL, 1, "use terminal please");
}

int	main(int ac, char **av, char **env)
{
	t_main	shell;

	check_ac(ac);
	init_shell(&shell, env);
	ft_malloc((size_t) & shell, SET_SHELL);
	while (av[0])
	{
		init_signal();
		shell.input = readline("minishell> ");
		if (check_exit(shell.input, &shell), shell.input)
			add_history(shell.input);
		if (check_syntax(shell.input))
			continue ;
		shell.tkn_list = tokenize_input(shell.input);
		if (free(shell.input), !shell.tkn_list)
			continue ;
		solve_expansion(shell.tkn_list, shell.env_list);
		shell.cmd_list = parse_tkn_to_cmds(shell.tkn_list, shell.env_list);
		if (g_heredoc_interrupted)
			continue ;
		if (shell.cmd_list && shell.cmd_list->args && !shell.cmd_list->next)
			execute_command(shell.cmd_list, &shell.env_list);
		else if (shell.cmd_list)
			execute_piped_commands(shell.cmd_list, shell.env_list);
	}
}
