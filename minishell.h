#ifndef MINISHELL_H
# define MINISHELL_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 4
# endif

# include "libft/libft.h"
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdbool.h>
# include <fcntl.h>
# include <string.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <errno.h>
# include <signal.h>
#include <sys/wait.h>

extern int	g_exit_status;

// Token type
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
	char	**env_arr; 				// t_env to char ** for execve()
	int		exit_status;
	int		pipe_fd[2];
	int		stdin_backup;
	int		stdout_backup;
	pid_t	last_pid;
	bool	is_pipe;
}	t_exec;

typedef struct s_lexer_vars			// Lexer variables
{
	int		i;
	bool	space;
	bool	heredoc;
}	t_lexvars;


typedef struct	s_token_vars		// Add token var
{
	char	*value;
	int		type;
	bool	space;
	bool	quoted;
}	t_token_vars;

typedef struct s_expand_vars		// Normal expanding vars
{
	t_token	**token;
	int		i;
	char	*input;
	t_env	*env;
	bool	*space;
	bool	heredoc;
}	t_expndvars;

typedef struct s_expand_str_vars	// Expnade inside quotes var
{
	char	*str;
	int		i;
	t_env	*env;
	char	*result;
	bool	heredoc;
}	t_exp_strvars;

typedef struct s_quote_vars			// Quotes vars
{
	char	*input;
	int		start;
	char	**output;
	t_env	*env;
	bool	*heredoc;
}	t_quotevars;

typedef struct s_operator_vars		// Operator check vars
{
	char		*input;
	t_token		**token;
	bool		space;
	bool		*heredoc;
	int			i;
}	t_oprvars;

//env_func
t_env	*collect_env(char **env);
char	*ft_getenv(char *key, t_env *env);

// parsing
int	is_ambiguous_redirection(t_cmd *cmd, t_token_type new_type);
int		check_quote(char *input);
int		inside_quote(t_quotevars *qt_var);
t_token	*creat_token(char *input, t_token_type type, bool space, bool quote);
void	add_token(t_token **token, t_token_vars *vars);
int		check_operator(t_oprvars *op_vars);
int 	expanding_var(t_expndvars *exp_var);
char	*expand_var_str(char *str, t_env *env, bool heredoc);
void	merge_words(t_token **token);
t_cmd	*parse_cmd(t_token **token);
int 	lexer_input(t_token **token, char *input, t_env *env);

int		handle_expansion(t_token **token, char *input, t_env *env, t_lexvars *st);
int 	handle_operator(t_token **token, char *input, int i, t_lexvars *st);
int 	handle_word(t_token **token, char *input, t_lexvars *st);
char	*extract_quoted_content(char *input, int start, int *end);
char	*handle_quote_expansion(char *str, char quote, t_env *env, bool heredoc);

int		is_only_space(char *filename);
int 	is_oper(int type);
t_cmd	*alloc_new_cmd(void);
char	**handl_word(char **args, char *new_arg);
char	**handl_word(char **args, char *new_arg);
int 	is_valid_redir_filename(t_token *op_token);
void	add_redirection(t_redir **redir_list, t_redir *new_redir);
int 	create_and_add_redir(t_cmd *cmd, t_token *redir_token);
int 	handle_redirection(t_cmd *cmd, t_token **curr_token);


// helper function
void	write_error(int	n);
void	cleanup(t_token *token, t_cmd *cmd, char *input, t_env *env);
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

// debug
void	print_tokens(t_token *token);
void	print_env(t_env *env);
void	print_array(char **arr);
void	print_cmds(t_cmd *cmd);
void    ll(void);

// Builtins
int		is_builtin(t_cmd *cmd);
int	run_builtin(t_cmd *cmd, t_env **env, t_exec *exec);
int	builtin_fd_opener(t_redir *r);
void	builtin_r_err(char *err_msg, char *f_name, int err);
int	builtin_append(t_redir *r, bool last_flag);
int	builtin_output(t_redir *r, bool last_flag);
int	builtin_input(t_redir *r);
int	builtin_check_execute(t_cmd *cmd, t_exec *exec, t_env **env);
int	builtin_r_preperation(t_cmd *cmd, t_exec *exec);
int	builtin_open_all_r(t_redir *redir);
bool	is_last_output_redir(t_redir *curr);
bool	is_last_input_redir(t_redir *curr, t_token_type type);
int	cmd_list_size(t_cmd *cmd);
void	builtin_reset_fds(t_exec *exec);

int	ft_cd(char **args, t_env **env_ptr);
int	ft_echo(t_cmd *cmd);
int	ft_env(t_env **env);

int	ft_exit(t_cmd *cmd, t_env **env);

void	replace_env_value(t_env *tmp, char *value);
void	append_env_value(t_env *tmp, char *value);
void	add_to_env(t_env **env_lst, char *key, char *value);
char	*get_value(char *str);
char	*get_key(char *str);
// int	ft_export(char **args, t_env **env_list, t_exec *exec);
int	ft_pwd(t_env **env);
int ft_unset(char **cmd, t_env **env);

// export uti


// ========== EXEC ===========

//utils
void	custom_error(char *cmd, char *error, int status);
// char	**env_to_array(t_env *env_head);
char	**env_to_array(t_env *lst);

// GNL
char	*get_next_line(int fd);
char	*gnl_read_line(int fd, char *stash, int bytes_read);
char	*get_my_line(char *stash, int i);
char	*modify_stash(char *stash, int i);
int		gnl_strlen(char *str);
char	*gnl_strjoin(char *stash, char *buffer);
char	*gnl_strchr(char *str, int c);
void	gnl_free_all(char *stash, char *buffer);
void	*gnl_calloc(size_t count, size_t size);
char	*gnl_strdup(char *s1);

// exec_main.c
int	execution_main(t_exec *exec, t_cmd *cmd_list, t_env *env);
void	get_last_status(pid_t last_pid);
void	parent_cleanup(int *tmp_in, t_exec *exec, t_cmd *cmd);
void 	ft_close_redir(t_redir *redi);
int		create_pipe(t_exec *exec);

// exec_child.c
void	child_process(t_exec *exec, t_cmd *cmd, int in_fd, t_env *env);
void	exec_external(t_cmd *cmd, t_exec *exec);
void	handle_redir(t_cmd *cmd);
bool	is_dir(const char *path);

// exec_child_utils
void	cleanup_dir_error(char **exec_path, char **command, t_cmd *cmd);
void	cleanup_malloc_error(char *exec_path, char *command);
void	cleanup_exec_error(char **exec_path, char **command, char **env_array);
void	ft_free_array(char **array);
void	ft_print_exit(t_cmd *cmd, int no_file);

// exec_redi
int		open_file(char *filename, int mode, bool quoted);
void	handle_input(t_redir *r);
void	handle_output(t_redir *r);
void	handle_append(t_redir *r);
void	handle_heredoc(t_redir *r);

// path resolve
char	*resolve_path(char *cmd, t_env *env, int *no_file);
char	*check_if_direct_path_valid(char *p);
char	*search_command_in_cwd(char *cmd);
void	search_command_in_path_dirs(char **full_path, char **path_dirs, char *cmd);
char	*get_path_value_from_env(t_env *env);

char	**split_t(const char *str, char sep);

// heredoc
int		check_heredocs(t_cmd *first_cmd, t_env *env_lst);
int		handle_one_heredoc(t_redir *r, t_env *env, bool last);
void	parent_heredoc(int *pipefd, pid_t pid, t_redir *r, bool last);
int		open_heredoc_child(t_redir *heredoc, int *pipefd, t_env *env, bool last);
char	*delim_join(char *s1, char *s2);

// heredoc utils
void	ft_read_line(char *delim, int *fd_pipe, t_redir *r, t_env *env, bool last);


// sigs
void	setup_heredoc_sig(void);
void	restore_sigs(void);
void	setup_parent_heredoc_sigs(void);
void	setup_parent_waiting_sigs(void);
void	setup_child_sigs(void);
void	handle_sigint(int sig);
void	handle_parent_sigint(int sig);
void	handle_parent_heredoc_sigint(int sig);
void	handle_heredoc_sigint(int sig);
void    handle_signals(void);


// expand heredoc
int	handle_heredoc_break(char *line, char *delim);
/* Expansion and string helpers */
char	*ft_strjoin_free(char *s1, char *s2);
void	ft_expand_vars_in_str(char **str, t_env *env);


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
