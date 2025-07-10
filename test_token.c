#include "minishell.h"

char	*get_token_type_str(t_tokentype type)
{
	if (type == WORD)
		return ("WORD");
	if (type == PIPE)
		return ("PIPE");
	if (type == R_IN)
		return ("R_IN");
	if (type == R_OUT)
		return ("R_OUT");
	if (type == APPEND)
		return ("APPEND");
	if (type == HERDOC)
		return ("HEREDOC");
	if (type == HERKEY)
		return ("HERKEY");
	if (type == R_FILE)
		return ("R_FILE");
	if (type == EXPAND)
		return ("EXPAND");
	if (type == D_QUOT)
		return ("D_QUOT");
	if (type == S_QUOT)
		return ("S_QUOT");
	return ("UNKNOWN");
}

void	print_token_list(t_token *head)
{
	while (head)
	{
		ft_putstr_fd("Token value: ", 1);
		ft_putstr_fd(head->value, 1);
		ft_putstr_fd(" | Type: ", 1);
		ft_putstr_fd(get_token_type_str(head->type), 1);
		ft_putstr_fd("\n", 1);
		head = head->next;
	}
}
