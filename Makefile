NAME = minishell
FLAGS =  -Wall -Wextra  -lreadline
SRC = main.c parsing.c helper_function.c

all: $(NAME)

$(NAME): $(SRC)
	cc $(SRC) $(FLAGS) -o $(NAME)

%.o: %.c
	$(CC) $(FLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: clean all