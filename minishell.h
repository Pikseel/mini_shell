/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mecavus <mecavus@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 12:25:10 by emrozmen          #+#    #+#             */
/*   Updated: 2025/07/11 15:22:32 by mecavus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <signal.h>
# include <unistd.h>
# include <stdlib.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <errno.h>
# include <string.h>
# include <readline/readline.h>
# include <readline/history.h>

# define ALLOC 0
# define CLEAR 1

# define PUSH 0
# define PULL 1

typedef enum e_tokentype
{
	WORD,
	PIPE,
	R_IN,
	R_OUT,
	R_FILE,
	HERKEY,
	HERDOC,
	APPEND,
	EXPAND,
	D_QUOT,
	S_QUOT,
}	t_tokentype;

typedef struct s_token
{
	struct	s_token	*next;
	struct	s_token	*prev;
	t_tokentype		type;
	char			*value;
	int				fd_rdir;
	int				is_removed;
	int				is_invalid_redir;
	/*int				is_heredoc_expand;*/
}	t_token;

typedef struct s_garbage
{
	void				*address;
	struct s_garbage	*next;

}	t_garbage;

typedef struct s_env {
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef struct s_command
{
	char				*command;
	char				**args;
	int					input_fd;
	int					output_fd;
	struct s_command	*next;
}	t_command;

typedef struct s_main
{
	char		*input;
	t_env		*env_list;
	t_token		*tkn_list;
	t_command	*cmd_list;
}	t_main;

char	*ft_strdup(char *src);
char	*ft_strchr(char *s, int c);
char	*ft_substr(char *s, unsigned int start, size_t len);
char	*ft_strjoin(char *s1, char *s2);
size_t	ft_strlcpy(char *dst, char *src, size_t dstsize);
size_t	ft_strlen(char *str);
size_t	ft_strlcat(char *s1, char *s2, size_t n);
void	ft_putstr_fd(char *s, int fd);
char	*ft_itoa(int n);
char	**ft_split(char *s);
char	**ft_split_sep(char *s, char c);
int		ft_isalpha(int c);
int		ft_isalnum(int c);
int		ft_isdigit(int c);
int		ft_is_space(char c);
int		ft_atoi(const char *str, int *flag);
int		skip_spaces(char *s, int i);
int		ft_strcmp(const char *s1, const char *s2);
char	*ft_strtrim(char *s, char c);

void	*ft_malloc(size_t size, int flag);

t_env	*lst_new(char *key, char *value);
t_env	*lst_last(t_env *lst);
void	lst_addback(t_env **lst, t_env *new);
t_env	*init_env_list(char **env);
int		count_env(t_env *env_list);
char	**env_list_to_array(t_env *env_list);
char	*get_env_value(t_env *env, const char *varname);

void	init_signal(void);

t_token	*tokenize_input(char *input);
int		count_word_tokens(t_token *tokens);
char	**tokens_to_args(t_token *tokens);
void	print_token_list(t_token *head);

void	solve_expansion(t_token *list, t_env *env);
char	*handle_exit_status(char *result, int *i);
char	*append_variable(char *result, const char *value, int *i, t_env *env);
char	*append_char(char *result, const char *value, int *i);
t_token	*create_token_from_word(char *word, t_token *original);
void	handle_word_splitting(t_token *current, char *expanded_value);
int		needs_word_splitting(char *expanded_value);

t_command	*parse_tokens_to_commands(t_token *tokens);
int			is_redirection(t_tokentype type);
t_token		*handle_redirect_token(t_token *token, t_command *cmd);
t_command	*create_command(void);
void		add_command(t_command **cmd_list, t_command *new_cmd);

int		is_builtin(char *command);
void	execute_command(char **args, t_env *env_list);
void	execute_builtin(char **args, t_env **env_list);
void	execute_external(char **args, t_env *env_list);
char	*find_command_path(char *command, t_env *env_list);
int		exit_status(int status, int flag);

void	setup_pipes(t_command *cmd_list);
void	execute_piped_commands(t_command *cmd_list, t_env *env_list);

void	builtin_echo(char **args);
void	builtin_cd(char **args, t_env *env_list);
void	builtin_pwd(void);
void	builtin_env(t_env *env_list);
void	builtin_export(char **args, t_env **env_list);
void	builtin_unset(char **args, t_env **env_list);
void	builtin_exit(char **args);

int		check_syntax(char *s);
void	check_ac(int ac);
int		check_exit(char *input);

#endif