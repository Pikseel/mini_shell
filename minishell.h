/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mecavus <mecavus@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 12:25:10 by emrozmen          #+#    #+#             */
/*   Updated: 2025/07/21 20:47:49 by mecavus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <signal.h>
# include <unistd.h>
# include <stdlib.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/wait.h>
# include <fcntl.h>

# define ALLOC 0
# define CLEAR 1
# define SET_SHELL 2
# define GET_SHELL 3

# define PUSH 0
# define PULL 1

# define START 0
# define END 1

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
	struct s_token	*next;
	struct s_token	*prev;
	t_tokentype		type;
	char			*value;
	int				fd_rdir;
	int				is_removed;
	int				is_invalid_redir;
}	t_token;

typedef struct s_garbage
{
	void				*address;
	struct s_garbage	*next;
}	t_garbage;

typedef struct s_env
{
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
	int					redirect_failed;
	struct s_command	*next;
}	t_command;

typedef struct s_main
{
	char		*input;
	t_env		*env_list;
	t_token		*tkn_list;
	t_command	*cmd_list;
}	t_main;

typedef struct s_delimiter_state
{
	int	s_quot;
	int	d_quot;
	int	i;
}	t_delimiter_state;

typedef struct s_quote_state
{
	int	s_quot;
	int	d_quot;
	int	found_quotes;
	int	i;
}	t_quote_state;

int			ft_is_space(char c);
int			skip_spaces(char *s, int i);
int			ft_isalpha(int c);
int			ft_isalnum(int c);
int			ft_isdigit(int c);
char		*ft_itoa(int n);
int			ft_atoi(const char *str, int *flag);
void		ft_putstr_fd(char *s, int fd);
char		**split_with_quotes(const char *s);
char		**ft_split_sep(char *s, char c);
char		**ft_split(char *s);
char		*ft_strchr( char *s, int c);
int			ft_strcmp(const char *s1, const char *s2);
char		*ft_strdup(char *src);
char		*ft_strjoin(char *s1, char *s2);
size_t		ft_strlcat(char *s1, char *s2, size_t n);
size_t		ft_strlcpy(char *dst, char *src, size_t dstsize);
size_t		ft_strlen(char *str);
char		*ft_strtrim(char *s, char c);
char		*ft_substr(char *s, unsigned int start, size_t len);
t_env		*lst_new(char *key, char *value);
void		lst_addback(t_env **lst, t_env *new);

void		builtin_cd(char **args, t_env *env_list);
void		builtin_echo(char **args);
void		builtin_env(t_env *env_list);
void		builtin_exit(char **args);
void		builtin_export(char **args, t_env **env_list);
void		builtin_pwd(void);
void		builtin_unset(char **args, t_env **env_list);

t_env		*init_env_list(char **env);
char		**env_list_to_array(t_env *env_list);

void		execute_external(char **args, t_env *env_list);
void		execute_external_piped(char **args, t_env *env_list);
char		*find_command_path(char *command, t_env *env_list);
void		free_env_array(char **env_array);
void		execute_builtin(char **args, t_env **env_list);
int			is_builtin(char *command);
void		execute_command(t_command *cmd, t_env **env_list);

char		*handle_exit_status(char *result, int *i);
char		*append_char(char *result, const char *value, int *i);
char		*append_variable(char *result, const char *value, int *i,
				t_env *env);
void		process_token_expansion(t_token *current, t_env *env);
char		*remove_quotes(const char *value, t_tokentype type);
char		*expand_assignment(const char *value, t_env *env, int *was_quoted);
int			is_assignment(const char *value);
char		*process_mixed_content(const char *value, t_env *env,
				int *was_quoted);
char		*get_env_value(t_env *env, const char *varname);
void		solve_expansion(t_token *list, t_env *env);

t_garbage	*lstlast_garbage(t_garbage *lst);
void		add_garbage(t_garbage **lst, t_garbage *new);
t_garbage	*new_garbage(void *adress);
void		clear_it(t_garbage *garbage);
void		clear_command_list(t_main *shell);
void		clear_pointers(t_main *shell);
void		*ft_malloc(size_t size, int flag);

char		*expand_heredoc_line(char *line, t_env *env_list);
void		heredoc_signal_handler(int sig);
void		init_heredoc_signal(void);
char		*process_heredoc_delimiter(char *delimiter);
int			should_expand_heredoc(char *original_delimiter);
char		*create_temp_filename(void);
int			write_heredoc_to_file(char *filename, char *delimiter,
				t_env *env_list, int expand);
int			handle_heredoc(t_token *current, t_env *env_list);

void		clear_exit(t_main *shell, int exit_code, char *message);

void		handle_redirections(t_token *current, t_command *cmd,
				t_env *env_list);
int			count_word_tokens(t_token *tokens);
t_command	*create_command(void);
void		add_command(t_command **cmd_list, t_command *new_cmd);
void		populate_command_args(t_command *cmd, t_token *start_tkn, int ac);
t_command	*parse_tkn_to_cmds(t_token *tokens, t_env *env_list);

int			count_commands(t_command *cmd_list);
void		fork_all_processes(t_command *cmd_list, pid_t *pids,
				t_env *env_list);
void		close_all_pipes(t_command *cmd_list);
void		wait_all_processes(pid_t *pids, int cmd_count);
void		execute_piped_child(t_command *cmd_list, t_command *current,
				t_env *env_list);
void		execute_piped_commands(t_command *cmd_list, t_env *env_list);

int			exit_status(int status, int flag);
void		init_signal(void);
void		ignore_signal(void);
void		execve_signal(void);

int			check_syntax(char *s);

t_tokentype	identify_token_type(char *str,	t_token *last);
t_token		*tokenize_input(char *input);

int			needs_word_splitting(char *expanded_value);
void		handle_word_splitting(t_token *current, char *expanded_value);
int			is_shell_operator(const char *s, int pos);
int			handle_quote_start(const char *s, int len, char *quote);
int			handle_quote_end(const char *s, int len, char *quote);
char		*extract_word(const char *s, int len);
void		skip_word(const char **s);
void		add_word_to_result(char **result, int *i, const char **s);
int			word_len(const char *s);
int			handle_quote(const char *s, int len, char *quote, int flag);

#endif