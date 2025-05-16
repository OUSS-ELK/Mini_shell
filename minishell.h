#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <readline/readline.h>
# include <readline/history.h>


typedef struct s_token
{
	char			*token;
	int				token_type;
	struct s_token	*next;
} t_token;

#endif