/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bel-abde <bel-abde@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 10:09:51 by bel-abde          #+#    #+#             */
/*   Updated: 2025/07/23 03:24:30 by bel-abde         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static t_redir	*get_last_heredoc(t_redir *r)
{
	t_redir	*last;

	last = NULL;
	while (r)
	{
		r->last = false;
		if (r->type == HEREDOC)
			last = r;
		r = r->next;
	}
	if (last)
		last->last = true;
	return (last);
}

static int	handle_heredocs_for_cmd(t_redir *redir, t_env *env_lst)
{
	t_redir	*r;

	r = redir;
	while (r)
	{
		if (r->type == HEREDOC)
		{
			if (handle_one_heredoc(r, env_lst) == -1 || g_exit_status == 1)
				return (0);
		}
		r = r->next;
	}
	return (1);
}

int	check_heredocs(t_cmd *first_cmd, t_env *env_lst)
{
	t_cmd	*cmd;
	t_redir	*last_heredoc;

	g_exit_status = 0;
	cmd = first_cmd;
	if (!cmd)
		return (1);
	while (cmd)
	{
		last_heredoc = get_last_heredoc(cmd->redir);
		setup_parent_heredoc_sigs();
		if (!handle_heredocs_for_cmd(cmd->redir, env_lst))
			return (0);
		restore_sigs();
		cmd = cmd->next;
	}
	return (1);
}

int	handle_one_heredoc(t_redir *r, t_env *env)
{
	int		pipefd[2];
	pid_t	pid;

	if (r == NULL || r->type != HEREDOC)
		return (1);
	setup_parent_heredoc_sigs();
	if (pipe(pipefd) == -1)
		return (custom_error(NULL, "pipe fail\n", 1), -1);
	pid = fork();
	if (pid == -1)
		return (custom_error(NULL, "fork fail\n", 1), -1);
	if (pid == 0)
		open_heredoc_child(r, pipefd, env);
	else
	{
		parent_heredoc(pipefd, pid, r);
		restore_sigs();
	}
	if (g_exit_status == 0)
		return (1);
	else
		return (-1);
}

void	parent_heredoc(int *pipefd, pid_t pid, t_redir *r)
{
	int		status;
	sig_t	old_handler;

	close(pipefd[1]);
	old_handler = signal(SIGINT, SIG_IGN);
	waitpid(pid, &status, 0);
	signal(SIGINT, old_handler);
	g_exit_status = WEXITSTATUS(status);
	if (g_exit_status == 0 && r->last)
		r->fd[0] = pipefd[0];
	else
	{
		close(pipefd[0]);
		r->fd[0] = -1;
	}
}
