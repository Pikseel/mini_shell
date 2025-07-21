/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mecavus <mecavus@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 16:50:00 by mecavus           #+#    #+#             */
/*   Updated: 2025/07/21 16:32:36 by mecavus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	*get_heredoc_interrupted(void)
{
	static int	g_heredoc_interrupted = 0;

	return (&g_heredoc_interrupted);
}

void	heredoc_signal_handler(int sig)
{
	int	*interrupted;

	(void)sig;
	interrupted = get_heredoc_interrupted();
	*interrupted = 1;
	ft_putstr_fd("\n", 1);
	exit_status(130, PUSH);
	close(STDIN_FILENO);
}

void	init_heredoc_signal(void)
{
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, &heredoc_signal_handler);
}
