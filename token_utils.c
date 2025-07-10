/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mecavus <mecavus@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 12:00:00 by mecavus          #+#    #+#             */
/*   Updated: 2025/07/10 12:00:00 by mecavus         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_word_tokens(t_token *tokens)
{
	int		count;
	t_token	*current;

	count = 0;
	current = tokens;
	while (current && current->type != PIPE)
	{
		if (current->type == WORD)
			count++;
		current = current->next;
	}
	return (count);
}

char	**tokens_to_args(t_token *tokens)
{
	char	**args;
	int		count;
	int		i;
	t_token	*current;

	count = count_word_tokens(tokens);
	args = ft_malloc(sizeof(char *) * (count + 1), ALLOC);
	if (!args)
		return (NULL);
	i = 0;
	current = tokens;
	while (current && current->type != PIPE)
	{
		if (current->type == WORD)
		{
			args[i] = ft_strdup(current->value);
			i++;
		}
		current = current->next;
	}
	args[i] = NULL;
	return (args);
}
