/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bel-abde <bel-abde@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 07:22:32 by bel-abde          #+#    #+#             */
/*   Updated: 2025/07/26 07:29:21 by bel-abde         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 4
# endif
# include <stdio.h>
# include "libft/libft.h"
# include <errno.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <unistd.h>

extern int			g_exit_status;

typedef enum s_token_type
{
	WORD = 1,
	PIPE,
	REDIR_IN,
	REDIR_OUT,
	APPEND,
	HEREDOC
}					t_token_type;

typedef struct s_token
{
	char			*token;
	t_token_type	type;
	bool			space;
	bool			quote;
	struct s_token	*next;
}					t_token;

typedef struct s_redir
{
	char			*filename;
	t_token_type	type;
	bool			quoted;
	int				fd[2];
	bool			last;
	struct s_redir	*next;
}					t_redir;

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}					t_env;

typedef struct s_cmd
{
	char			**args;
	t_redir			*redir;
	struct s_cmd	*next;
}					t_cmd;

typedef struct s_exec
{
	t_env			*env_lst;
	char			**env_arr;
	int				exit_status;
	int				pipe_fd[2];
	int				stdin_backup;
	int				stdout_backup;
	pid_t			last_pid;
	bool			is_pipe;
	char			*delim;
}					t_exec;

typedef struct s_lexer_vars
{
	int				i;
	bool			space;
	bool			heredoc;
}					t_lexvars;

typedef struct s_token_vars
{
	char			*value;
	int				type;
	bool			space;
	bool			quoted;
}					t_token_vars;

typedef struct s_expand_vars
{
	t_token			**token;
	int				i;
	char			*input;
	t_env			*env;
	bool			*space;
	bool			heredoc;
}					t_expndvars;

typedef struct s_simple_expand_vars
{
	t_token			**token;
	char			*input;
	int				start;
	t_env			*env;
	bool			*space;
}					t_spl_exp_vars;

typedef struct s_expand_str_vars
{
	char			*str;
	int				i;
	t_env			*env;
	char			*result;
	bool			heredoc;
}					t_exp_strvars;

typedef struct s_quote_vars
{
	char			*input;
	int				start;
	char			**output;
	t_env			*env;
	bool			*heredoc;
}					t_quotevars;

typedef struct s_operator_vars
{
	char			*input;
	t_token			**token;
	bool			space;
	bool			*heredoc;
	int				i;
}					t_oprvars;

t_env				*collect_env(char **env);
char				*ft_getenv(char *key, t_env *env);
int					parsing_function(t_token **token, char *input, t_cmd **cmd,
						t_env **envr);
int					lexer_input(t_token **token, char *input, t_env *env);
void				add_token(t_token **token, t_token_vars *vars);
t_cmd				*parse_cmd(t_token **token);
t_cmd				*alloc_new_cmd(void);
char				**handl_word(char **args, char *new_arg);
int					handle_redirection(t_cmd *cmd, t_token **curr_token);
int					check_quote(char *input);
int					handle_quote(t_token **token, char *input, t_env *env,
						t_lexvars *st);
int					valid_expand(char input, char next);
int					expansion(t_token **token, char *input, t_env *env,
						t_lexvars *st);
int					expanding_var(t_expndvars *exp_var);
char				*expand_var_str(char *str, t_env *env, bool heredoc);
void				ft_expand_vars_in_heredoc(char **str, t_env *env);
char				*f_strjoin_char(char *s, char c);
char				*f_strjoin_free(char *s1, char *s2);
char				*exit_status(char *result, int *i);
int					is_word_start(char c);
int					handle_word(t_token **token, char *input, t_lexvars *st);
int					handle_operator(t_token **token, char *input, int i,
						t_lexvars *st);
int					is_mergeable_words(t_token *token);
void				merge_words(t_token **token);
void				free_tokens(t_token *token);
void				free_env(t_env *env);
void				free_array(char **str);
void				free_redir(t_redir *redir);
void				free_cmd(t_cmd *cmd);
void				write_error(int n);
void				cleanup(t_token *token, t_cmd *cmd, char *input);
int					f_isspace(char c);
int					is_quote(char quote);
int					is_oper(int type);
int					is_operator(char oper);
int					all_space(char *input);
t_token				*get_last_token(t_token *token);
int					is_builtin(t_cmd *cmd);
int					run_builtin(t_cmd *cmd, t_env **env, t_exec *exec);
int					builtin_fd_opener(t_redir *r);
void				builtin_r_err(char *err_msg, char *f_name, int err);
int					builtin_append(t_redir *r, bool last_flag);
int					builtin_output(t_redir *r, bool last_flag);
int					builtin_input(t_redir *r);
int					builtin_check_execute(t_cmd *cmd, t_exec *exec,
						t_env **env);
int					builtin_r_preperation(t_cmd *cmd, t_exec *exec);
int					builtin_open_all_r(t_redir *redir);
bool				is_last_output_redir(t_redir *curr);
bool				is_last_input_redir(t_redir *curr, t_token_type type);
int					cmd_list_size(t_cmd *cmd);
void				builtin_reset_fds(t_exec *exec);
char				*ft_getenv_value(t_env *env, char *key);
int					get_args_count(char **args);
int					ft_cd(char **args, t_env **env_ptr);
int					resolve_cd_target(char **args, t_env *env, char **target,
						char *old_path);
int					resolve_cd_home_or_oldpwd(char **args, t_env *env,
						char **target);
char				*expand_tilde(char *input, t_env *env);
void				update_pwd_vars(t_env *env, char *old_path);
int					ft_echo(t_cmd *cmd);
int					ft_env(t_env **env);
int					ft_exit(t_cmd *cmd, t_env **env);
void				replace_env_value(t_env *tmp, char *value);
void				append_env_value(t_env *tmp, char *value);
void				add_to_env(t_env **env_lst, char *key, char *value);
char				*get_value(char *str);
char				*get_key(char *str);
int					ft_pwd(t_env **env);
int					ft_unset(char **cmd, t_env **env);
int					remove_env_node(t_env **head, t_env *target);
t_env				*envnew(char *key, char *value);
void				envadd_back(t_env **env, t_env *new);
int					set_env(t_env **env);
void				custom_error(char *cmd, char *error, int status);
char				**env_to_array(t_env *lst);
char				*get_next_line(int fd);
char				*gnl_read_line(int fd, char *stash, int bytes_read);
char				*get_my_line(char *stash, int i);
char				*modify_stash(char *stash, int i);
int					gnl_strlen(char *str);
char				*gnl_strjoin(char *stash, char *buffer);
char				*gnl_strchr(char *str, int c);
void				gnl_free_all(char *stash, char *buffer);
void				*gnl_calloc(size_t count, size_t size);
char				*gnl_strdup(char *s1);
int					execution_main(t_exec *exec, t_cmd *cmd_list, t_env *env);
void				get_last_status(pid_t last_pid);
void				parent_cleanup(int *tmp_in, t_exec *exec, t_cmd *cmd);
void				ft_close_redir(t_redir *redi);
int					create_pipe(t_exec *exec);
void				child_process(t_exec *exec, t_cmd *cmd, int in_fd,
						t_env *env);
void				exec_external(t_cmd *cmd, t_exec *exec);
void				handle_redir(t_cmd *cmd);
bool				is_dir(const char *path);
void				cleanup_dir_error(char **exec_path, char **command,
						t_cmd *cmd);
void				cleanup_malloc_error(char *exec_path, char *command);
void				cleanup_exec_error(char **exec_path, char **command,
						char **env_array);
void				ft_free_array(char **array);
void				print_n_exit(t_cmd *cmd, int no_file);
int					open_file(char *filename, int mode, bool quoted);
void				handle_input(t_redir *r);
void				handle_output(t_redir *r);
void				handle_append(t_redir *r);
void				handle_heredoc(t_redir *r);
char				*resolve_path(char *cmd, t_env *env, int *no_file);
char				*check_if_direct_path_valid(char *p);
char				*search_command_in_cwd(char *cmd);
void				search_command_in_path_dirs(char **full_path,
						char **path_dirs, char *cmd);
char				*get_path_value_from_env(t_env *env);
char				**split_t(const char *str, char sep);
int					check_heredocs(t_cmd *first_cmd, t_env *env_lst);
int					handle_one_heredoc(t_redir *r, t_env *env);
void				parent_heredoc(int *pipefd, pid_t pid, t_redir *r);
int					open_heredoc_child(t_redir *heredoc, int *pipefd,
						t_env *env);
char				*delim_join(char *s1, char *s2);
int					handle_heredoc_break(char *line, char *delim);
int					valide_exp_heredoc(char *line);
void				ft_read_line(t_exec *exec, t_redir *r);
void				setup_heredoc_sig(void);
void				restore_sigs(void);
void				setup_parent_heredoc_sigs(void);
void				setup_parent_waiting_sigs(void);
void				setup_child_sigs(void);
void				handle_sigint(int sig);
void				handle_parent_sigint(int sig);
void				handle_parent_heredoc_sigint(int sig);
void				handle_heredoc_sigint(int sig);
void				handle_signals(void);
int					handle_heredoc_break(char *line, char *delim);
char				*ft_strjoin_free(char *s1, char *s2);
char				*token_list_to_str(t_token *token);
void				print_export_error(const char *str);
char				**alloc_export_split(char *var);
int					get_value_start_index(char *var, int key_len);
char				**split_export_arg(char *var);
int					ft_export(char **av, t_exec *exec);
int					export_add_node(t_env **env_lst, char **splitted);
void				export_add_to_env(t_env **env, char ***splitted,
						char *has_value);
void				export_replace(t_env *var, char ***splitted,
						char *has_value);
int					export_valid_identifier(char *s);
char				*copy_str_n(char *dest, const char *src, size_t n);
void				print_export_vars(t_env *env_list);
char				*export_strcpy(char *dest, const char *src);
int					handle_export_logic(char **splitted, t_exec *exec,
						char *arg);
void				export_print(t_env *env);
void				export_append(t_env *var, char ***splitted);

# define RESET "\033[0m"
# define BOLDRED "\033[1;31m"

#endif
