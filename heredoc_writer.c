/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_writer.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mecavus <mecavus@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 16:23:07 by mecavus           #+#    #+#             */
/*   Updated: 2025/07/21 20:52:37 by mecavus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	process_heredoc_line(int fd, char *line, t_env *env_list,
				int expand)
{
	char	*expanded_line;

	if (expand)
	{
		expanded_line = expand_heredoc_line(line, env_list);
		write(fd, expanded_line, ft_strlen(expanded_line));
	}
	else
		write(fd, line, ft_strlen(line));
	write(fd, "\n", 1);
	return (0);
}

static int	handle_heredoc_child(int fd, char *processed_delimiter,
				t_env *env_list, int expand)
{
	char	*line;

	init_heredoc_signal();
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			ft_putstr_fd("minishell: warning: here-document delimited by ", 2);
			ft_putstr_fd("end-of-file (wanted `", 2);
			ft_putstr_fd(processed_delimiter, 2);
			ft_putstr_fd("')\n", 2);
			break ;
		}
		if (ft_strcmp(line, processed_delimiter) == 0)
		{
			free(line);
			break ;
		}
		process_heredoc_line(fd, line, env_list, expand);
		free(line);
	}
	close(fd);
	exit(0);
}

static int	handle_heredoc_parent(int fd, pid_t pid, char *filename)
{
	int	status;

	close(fd);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
		return (0);
	else if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		unlink(filename);
		exit_status(130, PUSH);
		rl_on_new_line();
		rl_replace_line("", 1);
		return (-1);
	}
	else
	{
		unlink(filename);
		return (-1);
	}
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
	if (pid == -1)
	{
		close(fd);
		return (-1);
	}
	if (pid == 0)
		handle_heredoc_child(fd, processed_delimiter, env_list, expand);
	else
		return (handle_heredoc_parent(fd, pid, filename));
	return (0);
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
