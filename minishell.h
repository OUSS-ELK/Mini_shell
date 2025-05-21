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
	WORD = 1, PIPE, REDIR_IN, REDIR_OUT, APPEND, HEREDOC
} t_token_type;

typedef struct s_token
{
	char			*token;
	t_token_type	type;
	struct s_token	*next;
} t_token;

#endif