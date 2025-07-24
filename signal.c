/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mecavus <mecavus@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 12:50:30 by emrozmen          #+#    #+#             */
/*   Updated: 2025/07/24 09:57:01 by mecavus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <signal.h>
#include <readline/readline.h>

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
	if (g_heredoc_interrupted)
		return ;
	(void)sig;
	ft_putstr_fd("\n", 1);
	rl_replace_line("", 1);
	rl_on_new_line();
	rl_redisplay();
	exit_status(130, PUSH);
}

void	init_signal(void)
{
	if (g_heredoc_interrupted)
		g_heredoc_interrupted = 0;
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, &handler);
}

void	ignore_signal(void)
{
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, SIG_IGN);
}

void	execve_signal(void)
{
	signal(SIGQUIT, SIG_DFL);
	signal(SIGINT, SIG_DFL);
}
