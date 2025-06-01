/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_case.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bel-abde <bel-abde@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 16:42:08 by bel-abde          #+#    #+#             */
/*   Updated: 2025/06/01 12:07:40 by bel-abde         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

t_token	*find_next_pipe(t_token *tokens)
{
	while (tokens)
	{
		if (tokens->type == PIPE)
			return (tokens);
		tokens = tokens->next;
	}
	return (NULL);
}

void	exec_child_process(t_token *cmd, t_exec *exec, int in_fd, int out_fd)
{
	if (in_fd != -1)
	{
		dup2(in_fd, STDIN_FILENO);
		close(in_fd);
	}
	if (out_fd != -1)
	{
		dup2(out_fd, STDOUT_FILENO);
		close(out_fd);
	}
	exec_cmd(cmd, exec);
	exit(exec->exit_status);
}

void	exec_pipe(t_token *tokens, t_exec *exec)
{
	t_token	*cmd;
	int		prev_fd;
	int		fd[2];
	int		status;

	cmd = tokens;
	prev_fd = -1;
	while (cmd)
	{
		exec_pipe2(cmd, exec, &prev_fd, fd);
		cmd = find_next_pipe(cmd);
		if (cmd)
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

void	exec_pipe2(t_token *cmd, t_exec *exec, int *prev_fd, int *fd)
{
	t_token	*next;
	pid_t	pid;

	next = find_next_pipe(cmd);
	if (next && pipe(fd) == -1)
		return ;
	if (next)
		next->type = -1;
	pid = fork();
	if (pid == 0)
	{
		if (next)
			exec_child_process(cmd, exec, *prev_fd, fd[1]);
		else
			exec_child_process(cmd, exec, *prev_fd, -1);
	}
	else
	{
		if (*prev_fd != -1)
			close(*prev_fd);
		if (next)
			close(fd[1]);
		if (next)
			*prev_fd = fd[0];
		else
			*prev_fd = -1;
	}
}
