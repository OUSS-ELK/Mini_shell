#include "minishell.h"
#include <sys/stat.h>   // for struct stat and S_ISDIR
#include <unistd.h> 
/*
 *  Forked child entry point (called from execution_main.c).
 *    - set signals back to default
 *    - wire up pipeline fds
 *    - process redirections
 *    - run builtin OR external command
 *    - exits on completion / failure
 */

static void	exec_builtin(t_cmd *cmd, t_env *env, t_exec *exec)
{
	if (cmd && cmd->args && cmd->args[0] && is_builtin(cmd) == 1) // sep
	{
		if (run_builtin(cmd, &env, exec)) // sep
			exit(g_exit_status);
	}
}

static void	print_heredoc_content_debug(t_cmd *cmd)
{
	t_redir	*r = cmd->redir;
	char	buf[1024];
	ssize_t	nbytes;

	while (r)
	{
		if (r->type == HEREDOC && r->fd[0] > 0)
		{
			fprintf(stderr, "DEBUG: Heredoc content for [%s]:\n", r->filename);
			fprintf(stderr, "DEBUG child: r->fd[0] = %d\n", r->fd[0]);
			// int tmp_fd = dup(r->fd[0]); // duplicate to avoid consuming it
			// if (tmp_fd == -1)
			// {
			// 	perror("dup failed");
			// 	r = r->next;
			// 	continue;
			// }
			// while ((nbytes = read(tmp_fd, buf, sizeof(buf) - 1)) > 0)
			// {
			// 	buf[nbytes] = '\0';
			// 	write(STDERR_FILENO, buf, nbytes);
			// }
			// close(tmp_fd);
		}
		r = r->next;
	}
}


void	child_process(t_exec *exec, t_cmd *cmd, int in_fd, t_env *env)
 {
	setup_child_sigs();
	if (in_fd != STDIN_FILENO && dup2(in_fd, STDIN_FILENO) == -1)
		custom_error(NULL, "dup2 fail", 1);
	if (cmd->next && dup2(exec->pipe_fd[1], STDOUT_FILENO) == -1)
		custom_error(NULL, "dup2 fail", 1);
	if (in_fd != STDIN_FILENO)
		close(in_fd);
	if (cmd->next)
	{
		close(exec->pipe_fd[0]);
		close(exec->pipe_fd[1]);
	}
	/* deal with <, >, >>, << */
	// print_heredoc_content_debug(cmd);
	handle_redir(cmd);
	exec_builtin(cmd, env, exec);
	exec_external(cmd, exec);
 }

// try to execute external cmd - exits the child on failure
void	exec_external(t_cmd *cmd, t_exec *exec)
{
	char	*cmd_name;
	char	*cmd_path;
	char	**envp;
	int		no_file;

	if (!cmd || !cmd->args || !cmd->args[0])
		exit(0);
	cmd_name = ft_strdup(cmd->args[0]);
	cmd_path = resolve_path(cmd->args[0], exec->env_lst, &no_file); // seperate file
	if (!cmd_path || !cmd_name[0])
		(free(cmd_name), ft_print_exit(cmd, no_file)); // print exit
	if (is_dir(cmd_path))
		cleanup_dir_error(&cmd_path, &cmd_name, cmd);
	envp = env_to_array(exec->env_lst); // for execve()
	if (!envp)
		cleanup_malloc_error(cmd_path, cmd_name);
	execve(cmd_path, cmd->args, envp);
	cleanup_exec_error(&cmd_path, &cmd_name, envp); // if we get here, execve failed - free and error
} // still need to be more robust?

void	handle_redir(t_cmd *cmd)
{
	t_redir	*r;

	r = cmd->redir;
	while (r)
	{
		if (r->type == REDIR_IN || r->type == HEREDOC)
			handle_input(r);
		else if (r->type == REDIR_OUT || r->type == APPEND)
			handle_output(r);
		r = r->next;
	}
}

// is the string given an existing directory?
bool	is_dir(const char *path)
{
	struct stat	tmp;

	if (!path || !*path || !ft_strchr(path, '/'))
		return (false);
	if (stat(path, &tmp) == -1)
		return (false);
	return (S_ISDIR(tmp.st_mode));
}
