/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils_2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bel-abde <bel-abde@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 09:05:02 by bel-abde          #+#    #+#             */
/*   Updated: 2025/07/21 09:05:11 by bel-abde         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
