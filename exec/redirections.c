#include "execution.h"

// Open input file or handle heredoc
static bool	handle_input(t_redir *redir, t_exec *exec)
{
	if (redir->type == HEREDOC)
	{
		if (expander(exec, redir))
			return (true);
		// normalement heredoc pipe already set up in redir->fd[0] by heredoc.
	}
	else
	{
		redir->fd[0] = open(redir->filename, MODE_INPUT);
		if (redir->fd[0] < 0)
		{
			perror(redir->filename);
			exec->exit_status = 1;
			return (true);
		}
	}
	return (false);
}

static bool	handle_output(t_redir *redir, t_exec *exec)
{
	int	flags;

	if (redir->type == APPEND)
		flags = MODE_APPEND;
	else
		flags = MODE_TRUNC;
	redir->fd[0] = open(redir->filename, flags, 0644);
	if (redir->fd[0] < 0)
	{
		perror(redir->filename);
		exec->exit_status = 1;
		return (true);
	}
	return (false);
}

// called by exec_cmd() before cmd is launched
bool	handle_redirections(t_cmd *cmd, t_exec *exec)
{
	t_redir	*redir;

	redir = cmd->redir;
	while (redir)
	{
		if ((redir->type == REDIR_IN || redir->type == HEREDOC)
			&& handle_input(redir, exec))
			return (true);
		else if ((redir->type == REDIR_OUT || redir->type == APPEND)
			&& handle_output(redir, exec))
				return (true);
		redir = redir->next;
	}
	return (false);
}