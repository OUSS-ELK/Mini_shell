/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bel-abde <bel-abde@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 14:59:01 by bel-abde          #+#    #+#             */
/*   Updated: 2025/07/21 08:52:23 by bel-abde         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	is_valid_n_flag(char *str)
{
	int	i;

	i = 2;
	if (str[0] != '-' || str[1] != 'n')
		return (0);
	while (str[i])
	{
		if (str[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

static char	*join_echo_args(char **args, int i)
{
	char	*result;
	char	*tmp;

	result = ft_strdup("");
	while (args[i])
	{
		tmp = ft_strjoin(result, args[i]);
		free(result);
		result = tmp;
		if (args[i + 1])
		{
			tmp = ft_strjoin(result, " ");
			free(result);
			result = tmp;
		}
		i++;
	}
	return (result);
}

static void	print_echo_output(char *msg, int suppress_newline)
{
	if (suppress_newline)
		ft_putstr_fd(msg, 1);
	else
		ft_putendl_fd(msg, 1);
}

int	ft_echo(t_cmd *cmd)
{
	int		i;
	int		n_flag;
	char	*output;

	i = 1;
	n_flag = 0;
	output = NULL;
	while (cmd->args[i] && ft_strncmp(cmd->args[i], "-n", 2) == 0)
	{
		if (!is_valid_n_flag(cmd->args[i]))
			break ;
		n_flag = 1;
		i++;
	}
	output = join_echo_args(cmd->args, i);
	if (!output)
		return (g_exit_status = 1, 1);
	print_echo_output(output, n_flag);
	free(output);
	return (g_exit_status = 0, 0);
}
