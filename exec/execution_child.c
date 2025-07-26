/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_child.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bel-abde <bel-abde@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 10:00:31 by bel-abde          #+#    #+#             */
/*   Updated: 2025/07/26 06:24:18 by bel-abde         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	exec_builtin(t_cmd *cmd, t_env *env, t_exec *exec)
{
	if (cmd && cmd->args && cmd->args[0] && is_builtin(cmd) == 1)
	{
		if (run_builtin(cmd, &env, exec))
		{
			exit(g_exit_status);
		}
	}
}

void	child_process(t_exec *exec, t_cmd *cmd, int in_fd, t_env *env)
{
	setup_child_sigs();
	if (in_fd != STDIN_FILENO && dup2(in_fd, STDIN_FILENO) == -1)
		custom_error(NULL, "dup2 fail", 1);
	if (cmd->next && dup2(exec->pipe_fd[1], STDOUT_FILENO) == -1)
		custom_error(NULL, "dup2 fail", 1);
	if (in_fd != STDIN_FILENO)
		close(in_fd);
	if (cmd->next)
	{
		close(exec->pipe_fd[0]);
		close(exec->pipe_fd[1]);
	}
	handle_redir(cmd);
	exec_builtin(cmd, env, exec);
	exec_external(cmd, exec);
}

void	exec_external(t_cmd *cmd, t_exec *exec)
{
	char	*cmd_name;
	char	*cmd_path;
	char	**envp;
	int		no_file;

	if (!cmd || !cmd->args || !cmd->args[0])
		exit(0);
	cmd_name = ft_strdup(cmd->args[0]);
	cmd_path = resolve_path(cmd->args[0], exec->env_lst, &no_file);
	if (!cmd_path || !cmd_name[0])
	{
		free(cmd_name);
		print_n_exit(cmd, no_file);
	}
	if (is_dir(cmd_path))
		cleanup_dir_error(&cmd_path, &cmd_name, cmd);
	envp = env_to_array(exec->env_lst);
	if (!envp)
		cleanup_malloc_error(cmd_path, cmd_name);
	execve(cmd_path, cmd->args, envp);
	cleanup_exec_error(&cmd_path, &cmd_name, envp);
}

void	handle_redir(t_cmd *cmd)
{
	t_redir	*r;

	r = cmd->redir;
	while (r)
	{
		if (r->type == REDIR_IN || r->type == HEREDOC)
			handle_input(r);
		else if (r->type == REDIR_OUT || r->type == APPEND)
			handle_output(r);
		r = r->next;
	}
}
