/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bel-abde <bel-abde@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 14:59:01 by bel-abde          #+#    #+#             */
/*   Updated: 2025/06/28 15:20:40 by bel-abde         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

/*
	-n is valid
	-nnn is valid
	-n -n is also valid
*/

static int	check_flag(char *s)
{
	int	i;

	if (!s || s[0] != '-')
		return (0);
	i = 1;
	if (s[i] != 'n')
		return (0);
	while (s[i])
	{
		if (s[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

static int	is_writable(t_exec *exec)
{
	if (write(1, NULL, 0) < 0)
	{
		exec->exit_status = 1;
		// print error (minishell echo write error)
		return (0);
	}
	return (1);
}

void	echo(char **av, t_exec *exec)
{
	int	print_newline;
	int	i;

	i = 1;
	print_newline = 1;
	if (!is_writable(exec))
		return ;
	while (av[i] && check_flag(av[i]))
	{
		print_newline = 0;
		i++;
	}
	while (av[i])
	{
		ft_putstr_fd(av[i], 1);
		if (av[i + 1])
			ft_putchar_fd(' ', 1);
		i++;
	}
	if (print_newline)
		ft_putchar_fd('\n', 1);
	exec->exit_status = 0;
}

