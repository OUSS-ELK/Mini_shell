/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bel-abde <bel-abde@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 15:45:56 by bel-abde          #+#    #+#             */
/*   Updated: 2025/06/01 15:54:58 by bel-abde         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

bool	expander(t_exec *exec, t_redir *redir)
{
	int		fd[2];
	char	*line;

	if (pipe(fd) < 0)
		return (true);
	if (write(fd[1], NULL, 0) < 0)
	{
		perror("heredoc write error");
		close(fd[0]);
		close(fd[1]);
		return (true);
	}
	while ((line = get_next_line(redir->fd)))
	{
		// function that expands the vars
		ft_putstr_fd(line, fd[1]);
		free(line);
	}
	close(fd[1]);
	close(redir->fd[0]);
	redir->fd[0] = fd[0];
	return (false);
}