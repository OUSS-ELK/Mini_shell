#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <readline/readline.h>
# include <readline/history.h>

typedef enum s_token_type
{
	WORD = 1,
	PIPE,
	REDIR_IN,
	REDIR_OUT,
	APPEND,
	HEREDOC
} t_token_type;

typedef struct s_token
{
	char			*token;
	t_token_type	type;
	struct s_token	*next;
} t_token;

// parsing
int		inside_quote(char *input, int start, t_token **token);
t_token	*create_token(char *input, t_token_type type);
void	add_token(t_token **token, char *input, t_token_type type);
int		check_operator(char *input, int i, t_token *token);
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

#endif