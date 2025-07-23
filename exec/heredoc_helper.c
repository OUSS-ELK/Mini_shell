/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_helper.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bel-abde <bel-abde@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 10:14:41 by bel-abde          #+#    #+#             */
/*   Updated: 2025/07/23 03:25:30 by bel-abde         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	open_heredoc_child(t_redir *heredoc, int *pipefd, t_env *env)
{
	char	*delim;
	t_exec	exec;

	close(pipefd[0]);
	setup_heredoc_sig();
	delim = NULL;
	delim = delim_join(heredoc->filename, "\n");
	if (!delim)
		custom_error(NULL, "heredoc delim fail\n", 1);
	exec.env_lst = env;
	exec.pipe_fd[0] = pipefd[0];
	exec.pipe_fd[1] = pipefd[1];
	exec.delim = delim;
	exec.env_arr = NULL;
	exec.exit_status = 0;
	exec.stdin_backup = -1;
	exec.stdout_backup = -1;
	exec.last_pid = 0;
	exec.is_pipe = false;
	ft_read_line(&exec, heredoc);
	free(delim);
	exit(0);
}

char	*delim_join(char *s1, char *s2)
{
	char	*new_str;
	size_t	total_len;
	size_t	i;
	size_t	j;

	if (!s1)
		return (ft_strdup(s2));
	if (!s2)
		return (ft_strdup(s1));
	total_len = ft_strlen(s1) + ft_strlen(s2) + 1;
	new_str = (char *)malloc(total_len * sizeof(char));
	if (!new_str)
	{
		free(s1);
		return (NULL);
	}
	i = -1;
	while (s1[++i])
		new_str[i] = s1[i];
	j = -1;
	while (s2[++j])
		new_str[i++] = s2[j];
	new_str[i] = '\0';
	free(s1);
	return (new_str);
}

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
	int	i;

	i = 0;
	while (line[i])
	{
		if (valid_expand(line[i], line[i + 1]))
			return (1);
		i++;
	}
	return (0);
}

void	ft_read_line(t_exec *exec, t_redir *r)
{
	char	*line;

	while (1)
	{
		write(STDOUT_FILENO, "> ", 2);
		line = get_next_line(STDIN_FILENO);
		if (handle_heredoc_break(line, exec->delim))
			break ;
		if (!r->quoted && valide_exp_heredoc(line))
		{
			ft_expand_vars_in_heredoc(&line, exec->env_lst);
		}
		if (r->last)
			write(exec->pipe_fd[1], line, ft_strlen(line));
		free(line);
	}
	close(exec->pipe_fd[1]);
}
