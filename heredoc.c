/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mecavus <mecavus@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 16:50:00 by mecavus           #+#    #+#             */
/*   Updated: 2025/07/17 16:57:49 by mecavus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	g_heredoc_interrupted = 0;

void	heredoc_signal_handler(int sig)
{
	(void)sig;
	g_heredoc_interrupted = 1;
	ft_putstr_fd("\n", 1);
	exit_status(130, PUSH);
	close(STDIN_FILENO);
}

void	init_heredoc_signal(void)
{
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, &heredoc_signal_handler);
}

char	*process_heredoc_delimiter(char *delimiter)
{
	char	*processed;
	int		len;
	int		i;
	int		j;

	if (!delimiter)
		return (NULL);
	len = ft_strlen(delimiter);
	if (len < 2)
		return (ft_strdup(delimiter));
	
	// Handle quoted delimiters
	if ((delimiter[0] == '"' && delimiter[len-1] == '"') ||
		(delimiter[0] == '\'' && delimiter[len-1] == '\''))
	{
		// Remove outer quotes
		processed = ft_malloc(len - 1, ALLOC);
		i = 1;
		j = 0;
		while (i < len - 1)
			processed[j++] = delimiter[i++];
		processed[j] = '\0';
		return (processed);
	}
	
	// Handle mixed quotes like "'emre'" -> 'emre'
	if (delimiter[0] == '"' && delimiter[len-1] == '"')
	{
		processed = ft_malloc(len - 1, ALLOC);
		i = 1;
		j = 0;
		while (i < len - 1)
			processed[j++] = delimiter[i++];
		processed[j] = '\0';
		return (processed);
	}
	
	return (ft_strdup(delimiter));
}

static int	should_expand_heredoc(char *original_delimiter)
{
	int	len;
	
	if (!original_delimiter)
		return (1);
	len = ft_strlen(original_delimiter);
	
	// If delimiter was quoted, don't expand
	if (len >= 2 && ((original_delimiter[0] == '"' && original_delimiter[len-1] == '"') ||
		(original_delimiter[0] == '\'' && original_delimiter[len-1] == '\'')))
		return (0);
	
	return (1);
}

static char	*expand_heredoc_line(char *line, t_env *env_list)
{
	char	*expanded;
	char	*result;
	int		i;
	int		len;

	if (!line)
		return (NULL);
	
	len = ft_strlen(line);
	result = ft_strdup("");
	i = 0;
	
	while (i < len)
	{
		if (line[i] == '$' && i + 1 < len)
		{
			if (line[i + 1] == '?')
			{
				expanded = handle_exit_status(result, &i);
				result = expanded;
			}
			else
			{
				expanded = append_variable(result, line, &i, env_list);
				result = expanded;
			}
		}
		else
		{
			expanded = append_char(result, line, &i);
			result = expanded;
		}
	}
	
	return (result);
}

static char	*create_temp_filename(void)
{
	static int	heredoc_count = 0;
	char		*num_str;
	char		*filename;
	char		*temp;

	num_str = ft_itoa(heredoc_count++);
	temp = ft_strjoin("/tmp/minishell_heredoc_", num_str);
	filename = ft_strjoin(temp, ".tmp");
	
	return (filename);
}

static int	write_heredoc_to_file(char *filename, char *delimiter, 
				t_env *env_list, int expand)
{
	int		fd;
	char	*line;
	char	*expanded_line;
	char	*processed_delimiter;

	fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd == -1)
		return (-1);
	
	processed_delimiter = process_heredoc_delimiter(delimiter);
	init_heredoc_signal();
	
	while (1)
	{
		line = readline("> ");
		if (!line || g_heredoc_interrupted)
		{
			if (g_heredoc_interrupted)
			{
				close(fd);
				unlink(filename);
				init_signal();
				return (-1);
			}
			break;
		}
		
		if (ft_strcmp(line, processed_delimiter) == 0)
		{
			free(line);
			break;
		}
		
		if (expand)
		{
			expanded_line = expand_heredoc_line(line, env_list);
			write(fd, expanded_line, ft_strlen(expanded_line));
		}
		else
		{
			write(fd, line, ft_strlen(line));
		}
		write(fd, "\n", 1);
		
		free(line);
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
	{
		return (-1);
	}
	
	fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		unlink(filename);
		return (-1);
	}
	
	// Store the fd in the token for later use
	current->fd_rdir = fd;
	// Store filename for cleanup
	current->value = filename;
	
	return (fd);
}

void	cleanup_heredoc_files(t_command *cmd_list)
{
	t_command	*current;

	current = cmd_list;
	while (current)
	{
		if (current->input_fd != STDIN_FILENO)
		{
			close(current->input_fd);
			// If it's a heredoc file, unlink it
			if (current->input_fd > 2)
			{
				// We can't directly know if it's a heredoc file
				// but we can try to unlink files starting with /tmp/minishell_heredoc_
			}
		}
		current = current->next;
	}
}
