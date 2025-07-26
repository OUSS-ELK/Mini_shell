/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bel-abde <bel-abde@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 02:25:06 by bel-abde          #+#    #+#             */
/*   Updated: 2025/07/26 07:32:32 by bel-abde         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*check_if_direct_path_valid(char *path)
{
	if (!path)
		return (NULL);
	if (*path == '/' || *path == '.')
	{
		if (access(path, X_OK) == 0)
			return (ft_strdup(path));
		else
			return (NULL);
	}
	return (NULL);
}

bool	is_dir(const char *path)
{
	struct stat	tmp;

	if (!path || !*path || !ft_strchr(path, '/'))
		return (false);
	if (stat(path, &tmp) == -1)
		return (false);
	return (S_ISDIR(tmp.st_mode));
}

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

void	custom_error(char *cmd, char *error, int status)
{
	g_exit_status = status;
	write(2, "mnshell: ", 9);
	if (cmd)
	{
		write(2, cmd, ft_strlen(cmd));
		write(2, ": ", 2);
	}
	write(2, error, ft_strlen(error));
	exit(status);
}

void	print_n_exit(t_cmd *cmd, int no_file)
{
	if (no_file)
		custom_error(cmd->args[0], "No such file or directory\n", 127);
	else
		custom_error(cmd->args[0], "command not found\n", 127);
}
