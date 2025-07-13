NAME = minishell
CC = cc

# Paths to Homebrew readline installation
BREW_READLINE = /goinfre/ouelkhar/homebrew/opt/readline
#-fsanitize=address
# werror
CFLAGS = -Wall -Wextra   -I. -Ilibft -I$(BREW_READLINE)/include
LDFLAGS = -Llibft -lft -L$(BREW_READLINE)/lib -lreadline

SRC_DIRS = parsing exec builtins utils

# Exclude specific files
EXCLUDED_FILES = builtins/export.c builtins/export_utils.c

# Collect source files, filtering out excluded ones from SRC_DIRS
SRC = main.c \
	get_next_line.c get_next_line_utils.c \
	$(filter-out $(EXCLUDED_FILES), $(shell find $(SRC_DIRS) -name '*.c'))

OBJ = $(SRC:.c=.o)
LIBFT = libft/libft.a

all: $(NAME)

$(NAME): $(LIBFT) $(OBJ)
	$(CC) $(OBJ) $(CFLAGS) $(LDFLAGS) -o $(NAME)

$(LIBFT):
	make -C libft

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)
	make clean -C libft

fclean: clean
	rm -f $(NAME)
	make fclean -C libft

re: fclean all