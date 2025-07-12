NAME = minishell
LIBFT = libft/libft.a
FLAGS =  -Wall -Wextra -lreadline -Llibft -lft -g
SRC = main.c parsing/main_parsing.c parsing/lexing.c parsing/lexing_helper.c parsing/helper_function.c parsing/environement.c parsing/env2.c parsing/parse.c

all: $(NAME)

$(LIBFT):
	make -C libft

$(NAME): $(SRC) $(LIBFT) minishell.h
	cc $(SRC) $(FLAGS) -o $(NAME)

%.o: %.c
	cc $(FLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)
	make clean -C libft

fclean: clean
	rm -f $(NAME)
	make fclean -C libft

re: clean all