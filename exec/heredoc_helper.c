/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_helper.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bel-abde <bel-abde@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 10:14:41 by bel-abde          #+#    #+#             */
/*   Updated: 2025/07/22 12:56:41 by bel-abde         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	handle_heredoc_break(char *line, char *delim)
{
	int	delim_len;

	if (!line)
		return (write(1, "\n", 1), 1);
	delim_len = ft_strlen(delim);
	if (ft_strncmp(line, delim, delim_len) == 0 && line[delim_len] == '\0')

	{
		free(line);
		line = NULL;
		g_exit_status = 0;
		return (1);
	}
	return (0);
}

int	valide_exp_heredoc(char *line)
{
	int i = 0;
	while (line[i])
	{
		if (valid_expand(line[i], line[i + 1]))
			return 1;
		i++;

	}
	return 0;
}

void	ft_read_line(char *delim, int *fd_pipe, t_redir *r, t_env *env, bool last)
{
	char	*line;

	while (1)
	{
		write(STDOUT_FILENO, "> ", 2);
		line = get_next_line(STDIN_FILENO);
		// printf("Line = %s\n", line);
		if (handle_heredoc_break(line, delim))
			break ;
		if (!r->quoted && valide_exp_heredoc(line))
		{
			// printf("Need to expand here.\n");
			ft_expand_vars_in_heredoc(&line, env);
		}
		if (last)
			write(fd_pipe[1], line, ft_strlen(line));
		// ssize_t w = write(fd_pipe[1], line, ft_strlen(line));
		// if (w == -1)
    	// perror("heredoc write error");
		// fprintf(stderr, "write returned %zd for line: %s", w, line);
		free(line);
	}
	close(fd_pipe[1]);
}


// helper: find last input type redir in list (redir_in or heredoc)
// static t_redir	*get_last_input(t_redir *redir)
// {
// 	t_redir	*last;
// 	t_redir	*r;

// 	last = NULL;
// 	r = redir;
// 	while (r)
// 	{
// 		if (r->type == REDIR_IN || redir->type == HEREDOC)
// 			last = r;
// 		r = r->next;
// 	}
// 	return (last);
// }

// // helper: find last redir_out or append
// static t_redir	*get_last_output(t_redir *redir)
// {
// 	t_redir	*last;
// 	t_redir	*r;

// 	last = NULL;
// 	r = redir;
// 	while (r)
// 	{
// 		if (r->type == REDIR_OUT || redir->type == APPEND)
// 			last = r;
// 		r = r->next;
// 	}
// 	return (last);
// }