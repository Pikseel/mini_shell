/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mecavus <mecavus@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 16:23:07 by mecavus           #+#    #+#             */
/*   Updated: 2025/07/22 20:19:50 by mecavus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	process_heredoc_line(int fd, char *line,
				t_env *env_list, int expand)
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

static char	*handle_quote_in_delimiter(char *result, char *delimiter,
				t_delimiter_state *state)
{
	if (delimiter[state->i] == '\'')
	{
		if (state->d_quot)
			result = append_char(result, delimiter, &state->i);
		else
		{
			state->s_quot = !(state->s_quot);
			state->i++;
		}
	}
	else if (delimiter[state->i] == '\"')
	{
		if (state->s_quot)
			result = append_char(result, delimiter, &state->i);
		else
		{
			state->d_quot = !(state->d_quot);
			state->i++;
		}
	}
	else
		result = append_char(result, delimiter, &state->i);
	return (result);
}

char	*process_heredoc_delimiter(char *delimiter)
{
	char				*result;
	t_delimiter_state	state;

	if (!delimiter)
		return (NULL);
	if (ft_strlen(delimiter) == 0)
		return (ft_strdup(delimiter));
	result = ft_strdup("");
	state.i = 0;
	state.s_quot = 0;
	state.d_quot = 0;
	while (delimiter[state.i])
		result = handle_quote_in_delimiter(result, delimiter, &state);
	return (result);
}

int	should_expand_heredoc(char *original_delimiter)
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

char	*create_temp_filename(void)
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
