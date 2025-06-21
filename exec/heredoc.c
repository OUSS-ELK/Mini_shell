
#include "execution.h"

/*
Scan tokens for heredocs.

Create a pipe and fork a child for each heredoc.

The child process collects heredoc input until it sees the delimiter.

That input is written into the write end of the pipe.

The parent closes the write end and uses the read end for redirection.

The redir->fd[0] stores the read-end of that pipe, so it can be used later for input redirection (e.g. in dup2()).

*/

// Go through all the tokens and apply handle_herdocs func on the right token
bool	exec_heredoc(t_cmd *cmds, t_exec *exec)
{
	t_cmd	*curr_cmd;

	curr_cmd = cmds;
	while (curr_cmd)
	{
		if (handle_heredoc_redir(curr_cmd->redir, exec))
			return (true);
		curr_cmd = curr_cmd->next;
	}
	// t_token	*curr;
	// t_redir	redir;

	// curr = tokens;
	// while (curr)
	// {
	// 	if (curr->type == HEREDOC && curr->next)
	// 	{
	// 		redir.type = HEREDOC;
	// 		redir.filename = curr->next->token;
	// 		redir.next = NULL;
	// 		if (handle_herdoc_redir(&redir, exec));
	// 			return (true);
	// 	}
	// 	curr = curr->next;
	// }
	return (false);
}

bool	handle_heredoc_redir(t_redir *redir, t_exec *exec)
{
	char	*tmp;
	int		fd;

	while (redir)
	{
		if (redir->type == HEREDOC)
		{
			tmp = ft_strdup(redir->filename);
			if (!tmp)
				return (true);
			fd = create_heredoc_pipe(tmp, exec);
			free(tmp);
			if (fd < 0)
				return (true);
			redir->fd[0] = fd;
			if (expander(exec, redir)) // Expand $VAR inside heredoc
			{
				close(redir->fd[0]);
				return (true);
			}
		}
		redir = redir->next;
	}
	// while (redir)
	// {
	// 	if (redir->type == HEREDOC)
	// 	{
	// 		tmp = ft_strdup(redir->filename);
	// 		if (!tmp)
	// 			return (true);
	// 		fd = create_herdoc_pipe(tmp, exec);
	// 		free(redir->filename);
	// 		free(tmp);
	// 		if (fd < 0)
	// 			return (true);
	// 		redir->fd[0] = fd;
	// 		if (expander(exec, redir))
	// 			{close(redir->fd); return (true);}
	// 	}
	// 	redir = redir->next;
	// }
	return (false);
}

int	create_heredoc_pipe(char *tmp, t_exec *exec)
{
	int		pipe_fd[2];
	pid_t	pid;
	int		status;

	if (pipe(pipe_fd) == -1)
		return (perror("pipe"), -1);
	pid = fork();
	if (pid < 0)
		return (perror("fork"), -1);
	if (pid == 0)
		heredoc_child(pipe_fd, tmp);
	waitpid(pid, &status, 0);
	// reset terminal mode here
	close(pipe_fd[1]); // close write end for parent
	if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
	{
		close(pipe_fd[0]);
		exec->exit_status = 1;
		return (-1);
	}
	return (pipe_fd[0]); // return read-end for redirection
}

static void	heredoc_child(int pipe_fd[2], char *tmp)
{
	char	*line;
	char	*tmp2;

	close(pipe_fd[0]);
	while (1)
	{
		line = readline("> ");
		if (!line || ft_strcmp(line, tmp) == 0)
			break ;
		tmp2 = ft_strjoin(line, "\n", NULL);
		if (tmp2)
		{
			write(pipe_fd[1], tmp2, ft_strlen(tmp2));
			free(tmp2);
		}
		free(line);
	}
	free(line);
	close(pipe_fd[1]);
	exit(0);
}
