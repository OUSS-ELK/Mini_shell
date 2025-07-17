#include "../minishell.h"

// open() simple wrapper
int	open_file(char *filename, int mode, bool quoted)
{
	int	fd;
	int	flags;

	if (!filename || *filename == '\0' || (!quoted && ft_strchr(filename, ' ')))
	{
		custom_error(filename, ": ambiguous redirect\n", 1);
		return (-1);
	}
	if (mode == 0)
		flags = O_CREAT | O_WRONLY | O_TRUNC;
	else if (mode == 1)
		flags = O_RDONLY;
	else
		flags = O_CREAT | O_WRONLY | O_APPEND;

	fd = open(filename, flags, 0644);
	if (fd == -1)
		custom_error(filename, ": No such file or directory.\n", 1);
	return (fd);
}

/* <  OR  <<  .  For “<<” we only link if it’s the last heredoc.             */
void	handle_input(t_redir *r)
{
	int	in_fd;

	if (r->type == REDIR_IN)                    /* “<” */
	{
		in_fd = open_file(r->filename, 1, r->quoted);
		if (dup2(in_fd, STDIN_FILENO) == -1)
		{
			close(in_fd);
			custom_error(NULL, "dup2 failure\n", 1);
		}
		close(in_fd);
	}
	else if (r->type == HEREDOC)
	{
		if (r->next == NULL ||                     /* last in list OR        */
			(r->next && (r->next->type != HEREDOC))) /* next is not another << */
			handle_heredoc(r);
	}
}

/* >    – truncate / overwrite.  Apply dup2 only for the last output redir. */
void	handle_output(t_redir *r)
{
	int	out_fd;

	if (r->type == REDIR_OUT)                   /* “>” */
	{
		out_fd = open_file(r->filename, 0, r->quoted);
		if (r->next == NULL)                    /* last output redirection */
		{
			if (dup2(out_fd, STDOUT_FILENO) == -1)
			{
				close(out_fd);
				custom_error(NULL, "dup2 failure\n", 1);
			}
		}
		close(out_fd);
	}
	else if (r->type == APPEND)                 /* “>>” */
		handle_append(r);
}


/* >> append.  Only dup2() if this redir is the **last** one in the chain. */
void	handle_append(t_redir *r)
{
	int	out_fd;

	out_fd = open_file(r->filename, 2, r->quoted);
	if (r->next == NULL)                        /* last output redirection */
	{
		if (dup2(out_fd, STDOUT_FILENO) == -1)
		{
			close(out_fd);
			custom_error(NULL, "dup2 failure\n", 1);
		}
	}
	close(out_fd);
}

void	handle_heredoc(t_redir *r)
{
	if (dup2(r->fd[0], STDIN_FILENO) == -1)
		custom_error(NULL, "dup2 failure (heredoc)\n", 1);
	close(r->fd[0]);
}
