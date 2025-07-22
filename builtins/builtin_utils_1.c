/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils_1.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bel-abde <bel-abde@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 09:03:33 by bel-abde          #+#    #+#             */
/*   Updated: 2025/07/21 09:08:34 by bel-abde         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	handle_fd_open_error(char *filename, int error_code)
{
	if (error_code == ENOENT)
		builtin_r_err("No such file or directory\n", filename, 1);
	else if (error_code == EISDIR)
		builtin_r_err("No such file or directory\n", filename, 1);
	else if (error_code == EACCES)
		builtin_r_err("No such file or directory\n", filename, 1);
	else
		builtin_r_err(strerror(error_code), filename, 1);
}

int	builtin_fd_opener(t_redir *r)
{
	int	fd;

	if (!r || !r->filename || r->filename[0] == '\0')
		return (builtin_r_err("minishell: : No such file or directory\n",
				r->filename, 1), -1);
	if (!r->quoted && ft_strchr(r->filename, ' '))
		return (builtin_r_err("minishell: ambiguous redirect\n", r->filename,
				1), -1);
	if (r->type == REDIR_OUT)
		fd = open(r->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (r->type == REDIR_IN)
		fd = open(r->filename, O_RDONLY);
	else if (r->type == APPEND)
		fd = open(r->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		return (-1);
	if (fd == -1)
		handle_fd_open_error(r->filename, errno);
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

int	builtin_check_execute(t_cmd *cmd, t_exec *exec, t_env **env)
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
		if (run_builtin(cmd, env, exec) == 1)
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
		return (0);
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
		return (-1);
	return (0);
}
