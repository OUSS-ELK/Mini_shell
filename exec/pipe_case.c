
#include "execution.h"

// Executes a child process with proper input/output redirection
void	exec_child_process(t_cmd *cmd, t_exec *exec, int in_fd, int out_fd)
{
	if (in_fd != -1)
	{
		if (dup2(in_fd, STDIN_FILENO) == -1)
			perror("dup2 in_fd");
		close(in_fd);
	}
	if (out_fd != -1)
	{
		if (dup2(out_fd, STDOUT_FILENO) == -1)
			perror("dup2 out_fd");
		close(out_fd);
	}
	exec_cmd(cmd, exec);
	exit(exec->exit_status);
}

// Sets up one part of the pipeline, forks and redirects
void	exec_pipe2(t_cmd *cmd, t_exec *exec, int *prev_fd, int *fd)
{
	pid_t	pid;
	bool	has_next;

	has_next = false;
	if (cmd->next != NULL)
		has_next = true;
	if (has_next)
	{
		if (pipe(fd) == -1)
		{
			perror("pipe");
			return;
		}
	}
	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		return;
	}
	if (pid == 0) // child process
	{
		if (has_next)
			exec_child_process(cmd, exec, *prev_fd, fd[1]);
		else
			exec_child_process(cmd, exec, *prev_fd, -1);
	}
	else // parent process
	{
		if (*prev_fd != -1)
			close(*prev_fd);
		if (has_next)
		{
			close(fd[1]);
			*prev_fd = fd[0];
		}
		else
			*prev_fd = -1;
	}
}

// Main pipeline executor: iterates through cmd linked list and sets up pipe chain
void	exec_pipe(t_cmd *cmds, t_exec *exec)
{
	t_cmd	*cmd;
	int		prev_fd;
	int		fd[2];
	int		status;

	cmd = cmds;
	prev_fd = -1;
	while (cmd)
	{
		exec_pipe2(cmd, exec, &prev_fd, fd);
		cmd = cmd->next;
	}
	while (wait(&status) > 0)
	{
		if (WIFEXITED(status))
			exec->exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			exec->exit_status = WTERMSIG(status) + 128;
	}
}
