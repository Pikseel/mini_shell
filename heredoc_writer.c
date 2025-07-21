/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_writer.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mecavus <mecavus@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 16:23:07 by mecavus           #+#    #+#             */
/*   Updated: 2025/07/21 19:13:56 by mecavus          ###   ########.fr       */
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
				close(fd);
				return (-1);
			}
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
	return (0);
}

int	write_heredoc_to_file(char *filename, char *delimiter,
			t_env *env_list, int expand)
{
	int		fd;
	char	*processed_delimiter;
	int		result;

	fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd == -1)
		return (-1);
	processed_delimiter = process_heredoc_delimiter(delimiter);
	init_heredoc_signal();
	result = handle_heredoc_input(fd, processed_delimiter, env_list, expand);
	if (result == -1 && g_heredoc_interrupted)
	{
		unlink(filename);
		init_signal();
		return (-1);
	}
	close(fd);
	init_signal();
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
