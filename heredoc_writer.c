/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_writer.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emrozmen <emrozmen@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 14:37:12 by emrozmen          #+#    #+#             */
/*   Updated: 2025/07/23 14:00:15 by emrozmen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_heredoc_input(int fd, char *processed_delimiter,
				t_env *env_list, int expand)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line || g_heredoc_interrupted)
		{
			if (g_heredoc_interrupted)
			{
				free(line);
				ft_malloc(0, CLEAR);
				exit(130);
			}
			if (!line)
				heredoc_putstr(processed_delimiter);
			break ;
		}
		if (ft_strcmp(line, processed_delimiter) == 0)
			return (free(line), ft_malloc(0, CLEAR), 0);
		process_heredoc_line(fd, line, env_list, expand);
	}
	return (0);
}

static void	write_heredoc_child(int fd, char *processed_delimiter,
				t_env *env_list, int expand)
{
	int	result;

	init_heredoc_signal();
	result = handle_heredoc_input(fd, processed_delimiter, env_list, expand);
	close(fd);
	if (result == -1)
		exit(130);
	else
		exit(0);
}

static int	write_heredoc_parent(pid_t pid, int fd, char *filename)
{
	int	status;

	ignore_signal();
	close(fd);
	waitpid(pid, &status, 0);
	init_signal();
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		unlink(filename);
		g_heredoc_interrupted = 1;
		return (-1);
	}
	if (WIFEXITED(status) && WEXITSTATUS(status) == 130)
	{
		unlink(filename);
		g_heredoc_interrupted = 1;
		return (-1);
	}
	return (0);
}

int	write_heredoc_to_file(char *filename, char *delimiter,
			t_env *env_list, int expand)
{
	int		fd;
	char	*processed_delimiter;
	pid_t	pid;

	fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd == -1)
		return (-1);
	processed_delimiter = process_heredoc_delimiter(delimiter);
	pid = fork();
	if (pid == 0)
		write_heredoc_child(fd, processed_delimiter, env_list, expand);
	else if (pid > 0)
		return (write_heredoc_parent(pid, fd, filename));
	close(fd);
	return (-1);
}

int	handle_heredoc(t_token *current, t_env *env_list)
{
	char	*filename;
	char	*delimiter;
	int		expand;
	int		fd;

	if (!current || !current->next || current->type != HERDOC)
		return (-1);
	delimiter = current->next->value;
	expand = should_expand_heredoc(delimiter);
	filename = create_temp_filename();
	if (write_heredoc_to_file(filename, delimiter, env_list, expand) == -1)
		return (-1);
	fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		unlink(filename);
		return (-1);
	}
	current->fd_rdir = fd;
	current->value = filename;
	return (fd);
}
