/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_main.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bel-abde <bel-abde@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 13:49:34 by bel-abde          #+#    #+#             */
/*   Updated: 2025/07/20 11:20:54 by bel-abde         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	handle_process(t_exec *exec, t_cmd *cmd_list, int *prev_pipe_read,
		t_env *env)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("mnshll: fork");
		g_exit_status = 1;
		return ;
	}
	if (pid == 0)
		child_process(exec, cmd_list, *prev_pipe_read, env);
	else
	{
		exec->last_pid = pid;
		parent_cleanup(prev_pipe_read, exec, cmd_list);
	}
}

int	execution_main(t_exec *exec, t_cmd *cmd_list, t_env *env)
{
	int	prev_pipe_read;

	prev_pipe_read = STDIN_FILENO;
	while (cmd_list)
	{
		if (!cmd_list->args || !cmd_list->args[0]
			|| cmd_list->args[0][0] == '\0')
		{
			cmd_list = cmd_list->next;
			continue ;
		}
		if (cmd_list->next && !create_pipe(exec))
			return (0);
		handle_process(exec, cmd_list, &prev_pipe_read, env);
		cmd_list = cmd_list->next;
	}
	setup_parent_waiting_sigs();
	get_last_status(exec->last_pid);
	restore_sigs();
	return (1);
}

void	get_last_status(pid_t last_pid)
{
	int		status;
	pid_t	pid;
	int		last_status;

	last_status = 0;
	pid = wait(&status);
	while (pid > 0)
	{
		if (pid == last_pid)
		{
			if (WIFEXITED(status))
				last_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
			{
				if (WTERMSIG(status) == SIGPIPE)
					last_status = 0;
				else
					last_status = 128 + WTERMSIG(status);
			}
		}
		pid = wait(&status);
	}
	g_exit_status = last_status;
}

void	parent_cleanup(int *tmp_in, t_exec *exec, t_cmd *cmd)
{
	if (*tmp_in != STDIN_FILENO)
		close(*tmp_in);
	if (cmd->next)
	{
		close(exec->pipe_fd[1]);
		*tmp_in = exec->pipe_fd[0];
	}
	ft_close_redir(cmd->redir);
}

void	ft_close_redir(t_redir *redi)
{
	while (redi)
	{
		if (redi->type == HEREDOC && (redi->fd[0] != -1 || redi->fd[1] != -1))
		{
			if (redi->fd[0] != -1)
			{
				close(redi->fd[0]);
				redi->fd[0] = -1;
			}
			if (redi->fd[1] != -1)
			{
				close(redi->fd[1]);
				redi->fd[1] = -1;
			}
		}
		redi = redi->next;
	}
}
