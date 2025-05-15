NAME = minishell
FLAGS =  -lreadline -lhistory -lncurses
SRC = main.c

all: $(NAME)

$(NAME): $(SRC)
	cc $(SRC) $(FLAGS) -o $(NAME)

clean:
	rm -f $(NAME)

re: clean all