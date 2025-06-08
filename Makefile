NAME = minishell
LIBFT = libft/libft.a
FLAGS =  -Wall -Wextra -lreadline -Llibft -lft
SRC = main.c parsing/parsing.c parsing/helper_function.c

all: $(NAME)

$(LIBFT):
	make -C libft

$(NAME): $(SRC) $(LIBFT)
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