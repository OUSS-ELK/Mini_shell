/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils_3.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bel-abde <bel-abde@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 09:05:47 by bel-abde          #+#    #+#             */
/*   Updated: 2025/07/26 07:40:13 by bel-abde         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
	int	count;

	count = 0;
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
