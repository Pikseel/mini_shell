/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mecavus <mecavus@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 12:50:30 by emrozmen          #+#    #+#             */
/*   Updated: 2025/07/10 15:33:26 by mecavus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exit_status(int status, int flag)
{
	static int	exit_status;
	if (flag == PUSH)
	{
		exit_status = status;
		if (exit_status < 0)
			exit_status = 256 + exit_status;
		if (exit_status > 255)
			exit_status = exit_status % 256;
	}
	return (exit_status);
}

static void	handler(int sig)
{
	(void)sig;
	rl_on_new_line();
	ft_putstr_fd("\n", 1);
	rl_redisplay();
	rl_replace_line("", 1);
	exit_status(130, PUSH);
}

static void	child_handler(int sig)
{
	if (sig == SIGINT)
	{
		ft_putstr_fd("\n", 1);
		exit_status(130, PUSH);
		exit(130);
	}
	else if (sig == SIGQUIT)
	{
		ft_putstr_fd("Quit (core dumped)\n", 2);
		exit_status(131, PUSH);
		exit(131);
	}
}

void	init_signal(void)
{
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, &handler);
}

void	init_child_signal(void)
{
	signal(SIGINT, child_handler);
	signal(SIGQUIT, child_handler);
}