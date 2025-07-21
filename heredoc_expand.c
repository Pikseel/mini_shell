/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_expand.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mecavus <mecavus@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 16:23:07 by mecavus           #+#    #+#             */
/*   Updated: 2025/07/21 16:32:36 by mecavus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*process_dollar_expansion(char *line, int *i, t_env *env_list)
{
	char	*result;

	result = ft_strdup("");
	if (line[*i] == '?')
		result = handle_exit_status(result, i);
	else if (ft_isalnum(line[*i]) || line[*i] == '_')
		result = append_variable(result, line, i, env_list);
	else
	{
		result = append_char(result, "$", i);
		(*i)--;
	}
	return (result);
}

char	*expand_heredoc_line(char *line, t_env *env_list)
{
	char	*result;
	char	*temp;
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
			temp = process_dollar_expansion(line, &i, env_list);
			result = ft_strjoin(result, temp);
		}
		else
			result = append_char(result, line, &i);
	}
	return (result);
}
