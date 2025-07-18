/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mecavus <mecavus@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 16:50:00 by mecavus           #+#    #+#             */
/*   Updated: 2025/07/18 17:22:11 by mecavus          ###   ########.fr       */
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
	char	*result;
	int		i;
	int		s_quot;
	int		d_quot;

	if (!delimiter)
		return (NULL);
	if (ft_strlen(delimiter) == 0)
		return (ft_strdup(delimiter));

	result = ft_strdup("");
	i = 0;
	s_quot = 0;
	d_quot = 0;

	while (delimiter[i])
	{
		if (delimiter[i] == '\'')
		{
			if (d_quot)
			{
				result = append_char(result, delimiter, &i);
			}
			else
			{
				s_quot = !s_quot;
				i++;
			}
		}
		else if (delimiter[i] == '\"')
		{
			if (s_quot)
			{
				result = append_char(result, delimiter, &i);
			}
			else
			{
				d_quot = !d_quot;
				i++;
			}
		}
		else
		{
			result = append_char(result, delimiter, &i);
		}
	}
	
	return (result);
}

static int	should_expand_heredoc(char *original_delimiter)
{
	int	i;
	
	if (!original_delimiter)
		return (1);
	i = 0;
	while (original_delimiter[i])
	{
		if (original_delimiter[i] == '"' || original_delimiter[i] == '\'')
			return (0);
		i++;
	}
	return (1);
}

static char	*expand_heredoc_line(char *line, t_env *env_list)
{
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
			i++;
			if (line[i] == '?')
			{
				result = handle_exit_status(result, &i);
			}
			else if (ft_isalnum(line[i]) || line[i] == '_')
			{
				result = append_variable(result, line, &i, env_list);
			}
			else
			{
				result = append_char(result, "$", &i);
				i--;
			}
		}
		else
		{
			result = append_char(result, line, &i);
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
	current->fd_rdir = fd;
	current->value = filename;
	return (fd);
}
