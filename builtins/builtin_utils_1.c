
#include "../minishell.h"


int	builtin_fd_opener(t_redir *r)
{
	int	fd;

	if (!r || !r->filename || r->filename[0] == '\0')
		return (builtin_r_err("minishell: : No such file or directory\n", \
		r->filename, 1), -1);
	// ambiguous redir (check later)
	if (!r->quoted && ft_strchr(r->filename, ' '))
		return (builtin_r_err("minishell: ambiguous redirect\n", \
		r->filename, 1), -1);
	if (r->type == REDIR_OUT)
		fd = open(r->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (r->type == REDIR_IN)
		fd = open(r->filename, O_RDONLY);
	else if (r->type == APPEND)
		fd = open(r->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		return (-1);
	if (fd == -1)
	{
		if (errno == ENOENT)
			builtin_r_err("No such file or directory\n", r->filename, 1);
		else if (errno == EISDIR)
			builtin_r_err("No such file or directory\n", r->filename, 1);
		else if (errno == EACCES)
			builtin_r_err("No such file or directory\n", r->filename, 1);
		else
			builtin_r_err(strerror(errno), r->filename, 1);
	}
	return (fd);
}

void	builtin_r_err(char *err_msg, char *f_name, int err)
{
	write(2, "minishell : ", 11);
	if (f_name)
	{
		write(2, f_name, ft_strlen(f_name));
		write(2, ": ", 2);
	}
	write(2, err_msg, ft_strlen(err_msg));
	g_exit_status = err;
}

int	builtin_append(t_redir *r, bool last_flag)
{
	int	fd;

	fd = builtin_fd_opener(r);
	if (fd == -1)
		return (0);
	if (last_flag)
		dup2(fd, STDOUT_FILENO);
	close(fd);
	return (1);
}

int	builtin_output(t_redir *r, bool last_flag)
{
	int	fd;

	if (r->type == REDIR_OUT)
	{
		fd = builtin_fd_opener(r);
		if (fd == -1)
			return (-1);
		if (last_flag)
			dup2(fd, STDOUT_FILENO);
		close(fd);
	}
	else if (r->type == APPEND)
		if (builtin_append(r, last_flag) == -1)
			return (-1);
	return (1);
}

int	builtin_input(t_redir *r)
{
	int	fd;

	if (r->type == REDIR_IN)
	{
		fd = builtin_fd_opener(r);
		if (fd == -1)
			return (-1);
		dup2(fd, STDIN_FILENO);
		close(fd);
	}
	return (1);
}

int	builtin_check_execute(t_cmd *cmd, t_exec *exec, t_env **env) // check return val with nmain func later
{
	if (!cmd || !cmd->args || !cmd->args[0])
		return (0);
	if (!is_builtin(cmd))
		return (0);
	if (cmd_list_size(cmd) == 1 && !exec->is_pipe)
	{
		if (cmd->redir && builtin_r_preperation(cmd, exec) == -1)
		{
			if (cmd->redir)
				builtin_reset_fds(exec);
			return (1);
		}
		if (run_builtin(cmd, env) == 1)
		{
			if (cmd->redir)
				builtin_reset_fds(exec);
			return (1);
		}
		builtin_reset_fds(exec);
		return (1);
	}
	return (0);
}

int	builtin_r_preperation(t_cmd *cmd, t_exec *exec)
{
	if (!cmd || !cmd->redir)
		return (0); // No redirection → success
	exec->stdin_backup = dup(STDIN_FILENO);
	if (exec->stdin_backup == -1)
	{
		perror("builtin stdin dup error");
		g_exit_status = 1;
		return (-1);
	}
	exec->stdout_backup = dup(STDOUT_FILENO);
	if (exec->stdout_backup == -1)
	{
		perror("builtin stdout dup error");
		g_exit_status = 1;
		return (-1);
	}
	if (builtin_open_all_r(cmd->redir) == -1)
		return (-1);  // ← was previously returning 1
	return (0);      // success
}

int	builtin_open_all_r(t_redir *redir)
{
	t_redir	*r;
	bool	is_last_output;

	r = redir;
	if (!redir)
		return (1);
	while (r)
	{
		if (r->type == REDIR_IN)
		{
			if (is_last_input_redir(r, r->type))
			{
				if (builtin_input(r) == -1)
					return (-1);
			}
		}
		else if (r->type == REDIR_OUT || r->type == APPEND)
		{
			is_last_output = is_last_output_redir(r);
			if (builtin_output(r, is_last_output) == -1)
				return (-1);
		}
		r = r->next;
	}
	return (1);
}

bool	is_last_output_redir(t_redir *curr)
{
	while (curr->next)
	{
		curr = curr->next;
		if (curr->type == REDIR_OUT || curr->type == APPEND)
			return (false);
	}
	return (true);
}

bool	is_last_input_redir(t_redir *curr, t_token_type type)
{
	while (curr->next)
	{
		curr = curr->next;
		if (curr->type == type)
			return (false);
	}
	return (true);
}

int	cmd_list_size(t_cmd *cmd)
{
	int	count = 0;
	while (cmd)
	{
		count++;
		cmd = cmd->next;
	}
	return (count);
}

void	builtin_reset_fds(t_exec *exec)
{
	if (dup2(exec->stdin_backup, STDIN_FILENO) == -1)
		perror("reset stdin");
	if (dup2(exec->stdout_backup, STDOUT_FILENO) == -1)
		perror("reset stdout");
	close(exec->stdin_backup);
	close(exec->stdout_backup);
	exec->stdin_backup = -1;
	exec->stdout_backup = -1;
}
