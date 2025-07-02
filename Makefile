NAME = minishell
LIBFT = libft/libft.a
FLAGS =  -Wall -Wextra -lreadline -Llibft -lft
SRC = main.c parsing/main_parse.c parsing/lexing.c parsing/helper_function.c parsing/environement.c parsing/parse.c

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