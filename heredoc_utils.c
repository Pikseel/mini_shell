/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mecavus <mecavus@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 16:23:07 by mecavus           #+#    #+#             */
/*   Updated: 2025/07/21 16:32:36 by mecavus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
				result = append_char(result, delimiter, &i);
			else
			{
				s_quot = !s_quot;
				i++;
			}
		}
		else if (delimiter[i] == '\"')
		{
			if (s_quot)
				result = append_char(result, delimiter, &i);
			else
			{
				d_quot = !d_quot;
				i++;
			}
		}
		else
			result = append_char(result, delimiter, &i);
	}
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
