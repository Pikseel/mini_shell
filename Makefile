CC = cc
CFLAGS = -Wall -Wextra -Werror
RM = rm -rf
NAME = minishell

SRC = main.c \
      env_utils.c \
      execute.c \
      execute_utils.c \
      execute_external.c \
      expand_variable.c \
      expansion.c \
      expansion_utils.c \
      expansion_utils2.c \
      expansion_tokens.c \
      garbage.c \
      garbage_utils.c \
      garbage_list.c \
      heredoc_signal.c \
      heredoc_utils.c \
      heredoc_expand.c \
      heredoc_writer.c \
      parser.c \
      parser_utils.c \
      parser_redirections.c \
      pipe.c \
      pipe_utils.c \
      signal.c \
      syntax.c \
      token_utils.c \
      token.c \
      word_split.c

BUILTIN_SRC = builtin/builtin_cd.c \
              builtin/builtin_echo.c \
              builtin/builtin_env.c \
              builtin/builtin_exit.c \
              builtin/builtin_export.c \
              builtin/builtin_pwd.c \
              builtin/builtin_unset.c

TOOLS_SRC = tools/ft_isalpha.c \
            tools/ft_is_space.c \
            tools/ft_itoa_atoi.c \
            tools/ft_putstr_fd.c \
            tools/ft_split.c \
            tools/ft_split_sep.c \
            tools/ft_split_quotes.c \
            tools/ft_split_quotes_utils.c \
            tools/ft_split_quotes_utils2.c \
            tools/ft_strchr.c \
            tools/ft_strcmp.c \
            tools/ft_strdup.c \
            tools/ft_strjoin.c \
            tools/ft_strlcat.c \
            tools/ft_strlcpy.c \
            tools/ft_strlen.c \
            tools/ft_strtrim.c \
            tools/ft_substr.c \
            tools/list.c

ALL_SRC = $(SRC) $(BUILTIN_SRC) $(TOOLS_SRC)

OBJ = $(ALL_SRC:.c=.o)

INCLUDES = minishell.h

all: $(NAME)

%.o: %.c $(INCLUDES)
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ) -lreadline

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re