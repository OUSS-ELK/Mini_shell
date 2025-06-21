#ifndef EXECUTION_H
# define EXECUTION_H

# include "../minishell.h"
# include <sys/wait.h>
# include <sys/stat.h>
# include <errno.h>
# include <stdbool.h>

# define MODE_TRUNC (O_CREAT | O_WRONLY | O_TRUNC)
# define MODE_APPEND (O_CREAT | O_WRONLY | O_APPEND)
# define MODE_INPUT O_RDONLY

extern int	g_signal_status;

// execute_main.c
void executor(t_cmd *cmds, t_exec *exec);

// cmd_or_pipe.c
bool	has_pipe(t_cmd *cmds);
void	cmd_or_pipe(t_cmd *cmds, t_exec *exec);
void	exec_cmd(t_cmd *cmd, t_exec *exec);
void	exec_cmd_helper(t_cmd *cmd, t_exec *exec);
void	exec_child(t_cmd *cmd, t_exec *exec);
void	free_cmd_list(t_cmd *cmd);

// pipe_case.c
void	exec_child_process(t_cmd *cmd, t_exec *exec, int in_fd, int out_fd);
void	exec_pipe2(t_cmd *cmd, t_exec *exec, int *prev_fd, int *fd);
void	exec_pipe(t_cmd *cmds, t_exec *exec);

// expand_heredoc.c
bool	expander(t_exec *exec, t_redir *redir);
char	*expand_line(const char *line, t_env *env_lst);
char	*get_env_value(const char *key, t_env *env_lst);



// redirections.c
bool	handle_redirections(t_cmd *cmd, t_exec *exec);

// heredoc.c
bool	exec_heredoc(t_cmd *cmds, t_exec *exec);
bool	handle_heredoc_redir(t_redir *redir, t_exec *exec);
int		create_heredoc_pipe(char *tmp, t_exec *exec);


// utils
void	free_2d(char **arr);

#endif