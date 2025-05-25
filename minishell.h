#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <readline/readline.h>
# include <readline/history.h>

// token type
typedef enum s_token_type
{
	WORD = 1,
	PIPE,
	REDIR_IN,
	REDIR_OUT,
	APPEND,
	HEREDOC
} t_token_type;

// token struct
typedef struct s_token
{
	char			*token;
	t_token_type	type;
	struct s_token	*next;
} t_token;

// environment variables
typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
} t_env;

// redirections
typedef struct s_redir
{
	char			*filename;
	t_token_type	type;
	int				fd[2];
	struct s_redir	*next;
} t_redir;


// parsing
int		inside_quote(char *input, int start, char **token);
t_token	*creat_token(char *input, t_token_type type);
void	add_token(t_token **token, char *input, t_token_type type);
int		check_operator(char *input, int i, t_token **token);
int 	find_word(char *input, int start, t_token **token);

// helper function
void	write_error(int	n);
void	free_tokens(t_token *token);
int 	f_strlen(char *s);
int 	f_isspace(char c);
int		is_quote(char quote);
int		is_operator(char oper);
char    *f_substring(char *s, int start, int len);
int		is_word_start(char c);
char	*f_strjoin(char *s1, char *s2);
char	*f_strdup(char *s);

// debug
void	print_tokens(t_token *token);

// === TEXT COLORS ===
#define RESET		"\033[0m"
#define BLACK		"\033[30m"
#define RED			"\033[31m"
#define GREEN		"\033[32m"
#define YELLOW		"\033[33m"
#define BLUE		"\033[34m"
#define MAGENTA		"\033[35m"
#define CYAN		"\033[36m"
#define WHITE		"\033[37m"

// === BOLD TEXT COLORS ===
#define BOLDRED     "\033[1;31m"
#define BOLDGREEN   "\033[1;32m"
#define BOLDYELLOW  "\033[1;33m"
#define BOLDBLUE    "\033[1;34m"
#define BOLDCYAN    "\033[1;36m"


#endif