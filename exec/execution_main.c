/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_main.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ouelkhar <ouelkhar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 13:49:34 by bel-abde          #+#    #+#             */
/*   Updated: 2025/07/17 23:25:09 by ouelkhar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// Check heredoc here first

int	execution_main(t_exec *exec, t_cmd *cmd_list, t_env *env)
{
	int		prev_pipe_read;
	pid_t	pid;


	prev_pipe_read = STDIN_FILENO;
	while (cmd_list)
	{
		// ✅ Skip empty commands (after expansion like: $NOTEXISTabc)
		if (!cmd_list->args || !cmd_list->args[0] || cmd_list->args[0][0] == '\0')
		{
			cmd_list = cmd_list->next;
			continue;
		}
		// create pupe only if there is a next cmd
		if (cmd_list->next && !create_pipe(exec))
			return (0);
		// printf("Forked\n");
		pid = fork();
			// printf("PID: %d", pid);
		if (pid == -1)
		{
			printf("fork error\n");
			perror("mnshll: fork error");
			g_exit_status = 1;
			return (0);
		}
		if (pid == 0) // child
			child_process(exec, cmd_list, prev_pipe_read, env);
		else // parent
		{
			exec->last_pid = pid;
			parent_cleanup(&prev_pipe_read, exec, cmd_list);
		}
		cmd_list = cmd_list->next;
	}
	setup_parent_waiting_sigs();
	get_last_status(exec->last_pid);
	restore_sigs();
	return (1);
}

// wait for all children and saves the status of last one
void	get_last_status(pid_t last_pid)
{
	int		status;
	pid_t	pid;
	int		last_status;

	last_status = 0;
	while ((pid = wait(&status)) > 0)
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
	}
	g_exit_status = last_status;
}

/*
 * Parent‑side clean‑up after forking one command in the pipeline.
 * - Closes the read‑end from the previous stage.
 * - Keeps the read‑end of the current pipe (write‑end is closed),
 *   so the next iteration can read from it.
 * - Frees / closes any heredoc fds attached to the current command.
 */

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

/*
 * Closes any open heredoc file descriptors in the redirection list.
 *
 * @param redi: Head of the redirection list
 */
void ft_close_redir(t_redir *redi)
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

/*
 * Wraps pipe(2) and stores the fds inside exec->pipe_fd.
 * Returns 1 on success, 0 on failure (sets g_exit_status).
 */
int	create_pipe(t_exec *exec)
{
	if (pipe(exec->pipe_fd) == -1)
	{
		custom_error(NULL, "pipe error\n", 1);
		g_exit_status = 1;
		return (0);
	}
	return (1);
}
