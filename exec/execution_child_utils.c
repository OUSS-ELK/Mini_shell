/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_child_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bel-abde <bel-abde@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 10:06:28 by bel-abde          #+#    #+#             */
/*   Updated: 2025/07/20 10:09:37 by bel-abde         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	cleanup_dir_error(char **exec_path, char **command, t_cmd *cmd)
{
	free(*exec_path);
	free(*command);
	custom_error(cmd->args[0], "is a directory\n", 126);
}

void	cleanup_malloc_error(char *exec_path, char *command)
{
	free(exec_path);
	free(command);
	custom_error(NULL, "Envp failure\n", 1);
}

void	cleanup_exec_error(char **exec_path, char **command, char **env_array)
{
	free(*exec_path);
	*exec_path = NULL;
	ft_free_array(env_array);
	*env_array = NULL;
	free(*command);
	*command = NULL;
	if (errno == EACCES)
		custom_error(NULL, "Permission denied\n", 126);
	else
		custom_error(NULL, "execution failure\n", 1);
}

void	ft_free_array(char **array)
{
	int	i;

	if (!array)
		return ;
	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

void	ft_print_exit(t_cmd *cmd, int no_file)
{
	if (no_file)
		custom_error(cmd->args[0], "No such file or directory\n", 127);
	else
		custom_error(cmd->args[0], "command not found\n", 127);
}
