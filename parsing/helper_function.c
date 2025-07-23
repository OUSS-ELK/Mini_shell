/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper_function.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ouelkhar <ouelkhar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 19:33:43 by ouelkhar          #+#    #+#             */
/*   Updated: 2025/07/23 05:28:22 by ouelkhar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	f_isspace(char c)
{
	return (c == ' ' || (c >= 9 && c <= 13));
}

int	is_oper(int type)
{
	return (type == REDIR_IN || type == REDIR_OUT || type == APPEND
		|| type == HEREDOC);
}

int	is_operator(char oper)
{
	return (oper == '|' || oper == '>' || oper == '<');
}

int	is_quote(char quote)
{
	return (quote == '\'' || quote == '"');
}
