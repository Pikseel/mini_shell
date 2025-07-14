/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_split.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mecavus <mecavus@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 14:00:00 by emrozmen          #+#    #+#             */
/*   Updated: 2025/07/14 17:05:25 by mecavus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	needs_word_splitting(char *expanded_value)
{
	int	i;

	if (!expanded_value)
		return (0);
	i = 0;
	while (expanded_value[i])
	{
		if (ft_is_space(expanded_value[i]))
			return (1);
		i++;
	}
	return (0);
}

static t_token	*create_token_from_word(char *word, t_token *original)
{
	t_token	*new_token;

	(void)original;
	new_token = ft_malloc(sizeof(t_token), ALLOC);
	if (!new_token)
		return (NULL);
	new_token->value = ft_strdup(word);
	new_token->type = WORD;
	new_token->next = NULL;
	new_token->prev = NULL;
	new_token->fd_rdir = -1;
	new_token->is_removed = 0;
	new_token->is_invalid_redir = 0;
	return (new_token);
}

static void	insert_remaining_words(t_token *current, char **words)
{
	t_token	*new_token;
	int		i;

	i = 1;
	while (words[i])
	{
		new_token = create_token_from_word(words[i], current);
		if (new_token)
		{
			new_token->next = current->next;
			new_token->prev = current;
			if (current->next)
				current->next->prev = new_token;
			current->next = new_token;
			current = new_token;
		}
		i++;
	}
}

static char	**prepare_words_for_splitting(char *expanded_value)
{
	char	*temp_str;
	char	**words;

	temp_str = ft_strdup(expanded_value);
	words = ft_split(temp_str);
	temp_str = NULL;
	return (words);
}

void	handle_word_splitting(t_token *current, char *expanded_value)
{
	char	**words;

	words = prepare_words_for_splitting(expanded_value);
	if (!words || !words[0])
	{
		current->value = expanded_value;
		current->type = WORD;
		return ;
	}
	expanded_value = NULL;
	current->value = ft_strdup(words[0]);
	current->type = WORD;
	insert_remaining_words(current, words);
	words = NULL;
}
