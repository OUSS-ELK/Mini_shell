NAME = minishell
CC = cc
READLINE = /goinfre/ouelkhar/homebrew/opt/readline
CFLAGS = -Wall -Wextra -Werror -I. -Ilibft -I$(READLINE)/include
LDFLAGS = -Llibft -lft -L$(READLINE)/lib -lreadline

SRC = main.c \
	parsing/cleaning.c parsing/environement.c parsing/environement_2.c \
	parsing/expand_handling.c parsing/expand_quote.c parsing/expand.c \
	parsing/helper_function.c parsing/helper_function2.c parsing/lexing.c \
	parsing/main_parsing.c parsing/parsing_helper.c parsing/parsing.c \
	parsing/quote_handling.c parsing/quote_helper.c parsing/word_opearator_handle.c \
	builtins/builtin_utils_1.c builtins/builtin_utils_2.c builtins/builtin_utils_3.c \
	builtins/cd_utils.c builtins/cd_utils_2.c builtins/cd.c \
	builtins/echo.c builtins/env.c builtins/exit.c \
	builtins/export_utils.c builtins/export_utils_2.c builtins/export_utils_3.c builtins/export.c \
	builtins/main_builtin.c builtins/pwd.c builtins/unset_utils.c builtins/unset.c \
	exec/exec_redir.c exec/execution_child_utils.c exec/execution_child.c \
	exec/execution_main.c exec/expand_heredoc.c exec/heredoc_helper.c exec/heredoc.c \
	exec/path_resolver.c exec/path_splitter.c exec/utils.c exec/utils_2.c \
	exec/utils_3.c exec/utils_4.c exec/utils_5.c \
	get_next_line.c get_next_line_utils.c \

OBJ = $(SRC:.c=.o)
LIBFT = libft/libft.a

all: $(NAME)

$(NAME): $(LIBFT) $(OBJ)
	$(CC) $(OBJ) $(CFLAGS) $(LDFLAGS) -o $(NAME)

$(LIBFT):
	make -C libft

%.o: %.c minishell.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)
	make clean -C libft

fclean: clean
	rm -f $(NAME)
	make fclean -C libft

re: fclean all

.PHONY: all clean fclean re
