#ifndef MINISHELL_H
# define MINISHELL_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 4
# endif

# include "libft/libft.h"
# include <stdio.h>
# include <sys/stat.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdbool.h>
# include <fcntl.h>
# include <string.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <errno.h>
# include <signal.h>
# include <sys/wait.h>

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
	bool			space;
	bool			quote;
	struct s_token	*next;
} t_token;

// redirections
typedef struct s_redir
{
    char            *filename;
    t_token_type    type;
    bool            quoted;                // if true not expand inside  
    int                fd[2];
    bool            last; // Added for heredoc
    struct s_redir    *next;
} t_redir;

// environment variables
typedef struct s_env
{
    char            *key;
    char            *value;
    struct s_env    *next;
} t_env;

// commands  struct that i should pass to exec
typedef struct s_cmd
{
    char            **args;
    t_redir            *redir;
    struct s_cmd    *next;
} t_cmd;

typedef struct s_exec
{
    t_env    *env_lst;
    char    **env_arr;                 // t_env to char ** for execve()
    int        exit_status;
    int        pipe_fd[2];
    int        stdin_backup;
    int        stdout_backup;
    pid_t    last_pid;
    bool    is_pipe;
    char    *delim; // Added for heredoc
}    t_exec;

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

typedef struct s_simple_expand_vars
{
	t_token		**token;
	char		*input;
	int			start;
	t_env		*env;
	bool		*space;
}	t_spl_exp_vars;

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
char	*ft_getenv_value(t_env *env, char *key);

t_env	*handle_special_env(char *env_var, int *skip);


/*			====	Parsing	& Lexing	====		*/ 
int		parsing_function(t_token **token, char *input, t_cmd **cmd, t_env **envr);
int 	lexer_input(t_token **token, char *input, t_env *env);
void	add_token(t_token **token, t_token_vars *vars);

/* 			====	Handling Quotes		====		*/
int		check_quote(char *input);
int		handle_quote(t_token **token, char *input, t_env *env, t_lexvars *st);

/* 			====	Expansion			====		*/
int		valid_expand(char input, char next);
int		expansion(t_token **token, char *input, t_env *env, t_lexvars *st);
int 	expanding_var(t_expndvars *exp_var);

/* 		==		Handling Word & operator	==		*/
int		is_word_start(char c);
int 	handle_word(t_token **token, char *input, t_lexvars *st);
int 	handle_operator(t_token **token, char *input, int i, t_lexvars *st);

/* 			====	Merging words		====		*/
int		is_mergeable_words(t_token *token);
void	merge_words(t_token **token);

/* 			====		Cleaning		====		*/
void	free_tokens(t_token *token);
void	free_env(t_env *env);
void	free_array(char **str);
void	free_redir(t_redir *redir);
void	free_cmd(t_cmd *cmd);


t_token	*creat_token(char *input, t_token_type type, bool space, bool quote);
char	*expand_var_str(char *str, t_env *env, bool heredoc);
t_cmd	*parse_cmd(t_token **token);
char	*f_strjoin_char(char *s, char c);
char	*f_strjoin_free(char *s1, char *s2);
char	*exit_status(char *result, int *i);


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
void	cleanup(t_token *token, t_cmd *cmd, char *input);
int 	f_isspace(char c);
int		is_quote(char quote);
int		is_operator(char oper);
int		all_space(char *input);
t_token	*get_last_token(t_token *token);

// // debug
// void	print_tokens(t_token *token);
// void	print_env(t_env *env);
// void	print_array(char **arr);
void	print_cmds(t_cmd *cmd);

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
int	ft_pwd(t_env **env);
int ft_unset(char **cmd, t_env **env);

// export uti


// ========== EXEC ===========

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
void	rint_n_exit(t_cmd *cmd, int no_file);

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

// EXEC HEREDOC
int check_heredocs(t_cmd *first_cmd, t_env *env_lst);
int handle_one_heredoc(t_redir *r, t_env *env);
void parent_heredoc(int *pipefd, pid_t pid, t_redir *r);
int open_heredoc_child(t_redir *heredoc, int *pipefd, t_env *env);
char *delim_join(char *s1, char *s2);
int handle_heredoc_break(char *line, char *delim);
int valide_exp_heredoc(char *line);
void ft_read_line(t_exec *exec, t_redir *r);


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
char	*token_list_to_str(t_token *token);
void	ft_expand_vars_in_heredoc(char **str, t_env *env);

// ------- EXPORT
void	print_export_error(const char *str);
int		is_valid_export_key(char *s);
char	**alloc_export_split(char *var);
int		get_value_start_index(char *var, int key_len);
char	**split_export_arg(char *var);
void	update_existing_var(t_env *var, char ***splitted, char *has_value);
int		add_new_env_var(t_env **env_lst, char **splitted);
void	add_env_if_missing(t_env **env, char ***splitted, char *has_value);
void	append_to_env_value(t_env *var, char ***splitted);
void	print_export_vars(t_env *env);
int		process_export_argument(char **splitted, t_exec *exec, char *arg);
int		ft_export(char **av, t_exec *exec);



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
