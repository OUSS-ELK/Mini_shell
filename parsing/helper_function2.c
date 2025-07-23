/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper_function2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bel-abde <bel-abde@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 03:48:46 by ouelkhar          #+#    #+#             */
/*   Updated: 2025/07/23 05:21:33 by bel-abde         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	write_error(int n)
{
	if (n == 1)
		write(2, "Parsing Error\n", 14);
	else if (n == 2)
		write(2, "Lexer Error\n", 12);
	else if (n == 3)
		write(2, "Quote Error\n", 12);
	else if (n == 4)
		write(2, "syntax error: missing filename after redirection\n", 49);
	else if (n == 5)
		write(1, "Syntax error empty command before pipe\n", 39);
	else if (n == 6)
		write(1, "Syntax error empty command after pipe\n", 38);
	else if (n == 7)
		write(1, "Parsing Syntax error ambiguous redirect\n", 40);
	else if (n == 8)
		write(1, "fatal fork/pipe error\n", 22);
	else
		write(2, "Error\n", 6);
}

void	cleanup(t_token *token, t_cmd *cmd, char *input)
{
	if (token)
		free_tokens(token);
	if (cmd)
		free_cmd(cmd);
	if (input)
		free(input);
}

t_token	*get_last_token(t_token *token)
{
	if (!token)
		return (NULL);
	while (token->next)
		token = token->next;
	return (token);
}

int	all_space(char *input)
{
	int	i;

	i = 0;
	if (!input)
		return (1);
	while (input[i])
	{
		if (!f_isspace(input[i]) && input[i] != ':')
			return (0);
		i++;
	}
	return (1);
}
