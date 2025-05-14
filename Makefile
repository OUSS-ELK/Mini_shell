NAME = minishell
FLAGS =  -lreadline

SRC = main.c

all: $(NAME)

$(NAME): $(SRC)
	cc $(SRC) $(FLAGS) -o $(NAME)

clean:
	rm -f $(NAME)

re: clean all