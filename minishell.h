#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdbool.h>
# include <fcntl.h>
# include <string.h>
# include <readline/readline.h>
# include <readline/history.h>

// globale exit status
// int	global_status = 0;

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
	bool			space;				// for merge words that are no space bitween them
	bool			quote;
	struct s_token	*next;
} t_token;

// redirections
typedef struct s_redir
{
	char			*filename;
	t_token_type	type;
	bool			quoted;				// if true not expand inside  
	int				fd[2];
	struct s_redir	*next;
} t_redir;

// environment variables
typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
} t_env;

// commands  struct that i should pass to exec
typedef struct s_cmd
{
	char			**args;
	t_redir			*redir;
	struct s_cmd	*next;
} t_cmd;

typedef struct s_exec
{
	t_env	*env_lst;
	char	**env_arr; // t_env to char ** for execve()
	int		exit_status;
	int		pipe_fd[2];
	int		stdin_backup;
	int		stdout_backup;
	pid_t	last_pid;
	bool	is_pipe;
}	t_exec;

typedef struct s_lexvars
{
	int		i;
	bool	space;
	bool	heredoc;
}	t_lexvars;


//env_func
t_env	*collect_env(char **env);
char	*ft_getenv(char *key, t_env *env);

// parsing
int		check_quote(char *input);
int		inside_quote(char *input, int start, char **output, t_env *env, bool *heredoc);
t_token	*creat_token(char *input, t_token_type type, bool space, bool quote);
void	add_token(t_token **token, char *input, t_token_type type, bool space, bool quote);
int		check_operator(char *input, int i, t_token **token, bool space, bool *heredoc);
int 	expanding_var(t_token **token, int i, char *input, t_env *env, bool *space, bool heredoc);
char	*expand_var_str(char *str, t_env *env, bool heredoc);
void	merge_words(t_token **token);
t_cmd	*parse_cmd(t_token **token);
int 	parsing_function(t_token **token, char *input, char **env, t_cmd **cmd);


// helper function
void	write_error(int	n);
void	free_cmd(t_cmd *cmd);
void	free_redir(t_redir *redir);
void	free_tokens(t_token *token);
void	free_env(t_env *env);
void	free_array(char **str);
int 	f_isspace(char c);
int		is_quote(char quote);
int		is_operator(char oper);
int		is_word_start(char c);
int		valid_expand(char input, char next);
int		is_alpha(char input);
int		all_space(char *input);
t_token	*get_last_token(t_token *token);

// int	handle_word(t_token **token, char *input, bool *space);


// debug
void	print_tokens(t_token *token);
void	print_env(t_env *env);
void	print_array(char **arr);
void	print_cmds(t_cmd *cmd);
void    ll(void);


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
