/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_or_pipe.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bel-abde <bel-abde@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 11:05:04 by bel-abde          #+#    #+#             */
/*   Updated: 2025/06/01 12:13:46 by bel-abde         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

bool	has_pipe(t_token *tokens)
{
	while (tokens)
	{
		if (tokens->type == PIPE)
			return true;
		tokens = tokens->next;
	}
	return false;
}

void	cmd_or_pipe(t_token *tokens, t_exec *exec)
{
	if (!tokens)
		return ;
	if (has_pipe(tokens))
		exec_pipe(tokens, exec);
	else
		exec_cmd(tokens, exec);
}

void	exec_cmd(t_token *tokens, t_exec *exec)
{
	char	**cmd_args;

	if (handle_redir(tokens, exec)) // to parse redir from tokens and set up fds etc..
		return ;
	cmd_args = tokens_to_args(tokens, exec->env_lst);
	if (!cmd_args || !cmd_args[0])
	{
		free_str_arr(cmd_args);
		return ;
	}
	if (exec->is_pipe)
	{
		if (dup2(exec->pipe_fd[1], STDOUT_FILENO) < 0)
		{
			free_str_arr(cmd_args);
			return ;
		}
		close(exec->pipe_fd[1]);
		close(exec->pipe_fd[0]);
	}
	if (is_builtin(cmd_args, exec)) // care
	{
		free_str_arr(cmd_args);
		return ;
	}
	cmd_args[0] = get_path(cmd_args[0], exec);
	if (!cmd_args[0])
	{
		free_str_arr(cmd_args);
		return ;
	}
	exec_cmd_helper(tokens, exec);
	free_str_arr(cmd_args); // Check later
}

void	exec_cmd_helper(t_token *cmd_args, t_exec *exec)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (fork_wrapper(pid)) // TO DO
		return ;
	if (pid == 0) //child
		exec_child(cmd_args, exec);
	waitpid(pid, &status, 0);
	// After the child finishes running, the parents needs to know what happened
	// Did the command succeed or fail? // Did the command get killed by a signal?
	// Needed also for "$?" to get last cmd exit status
	if (WIFEXITED(status))
		exec->exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		exec->exit_status = WTERMSIG(status) + 128;
	// reset std i/o files
	reset_io(exec); // TO DO
}

void	exec_child(t_token *tokens, t_exec *exec)
{
	char	**cmd_args;

	cmd_args = tokens_to_args(tokens, exec->env_lst);
	if (!cmd_args || !cmd_args[0])
		exit(1);
	execve(cmd_args[0], cmd_args, exec->env_arr);
	// If execve succeeds, this line is never reached

	free_str_arr(cmd_args);
	if (errno == ENOENT)
	{
		printf("command not found\n");
		exit(127);
	}
	else if (errno == EACCES)
	{
		printf("permission denied\n");
		exit(126);
	}
	else
		exit(1);
}

// void	exec_child(t_token *tokens, t_exec *exec)
// {
// 	char	**cmd_args;

// 	cmd_args = tokens_to_args(tokens, exec->env_lst);
// 	if (!cmd_args || !cmd_args[0])
// 		exit(1);
// 	execve(cmd_args[0], cmd_args, exec->env_arr);
// 	// if successful, never returns, process is replaced by the new program image so wbool	has_pipe(t_token *tokens)
// {
// 	while (tokens)
// 	{
// 		if (tokens->type == PIPE)
// 			return true;
// 		tokens = tokens->next;
// 	}
// 	return false;
// }e never reach what's below
// 	free_2d(cmd_args);
// 	if (errno = ENOENT)
// 	{
// 		printf("exit 127 error\n"); // to change later
// 		exit(127); // cmd not found
// 	}
// 	else if (errno == EACCES)
// 	{
// 		printf("exit 126 error\n"); // to change later
// 		exit(126); // perm denied
// 	}
// 	else
// 		exit(1);
// }
