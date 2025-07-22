/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_opearator_handle.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ouelkhar <ouelkhar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 02:31:05 by ouelkhar          #+#    #+#             */
/*   Updated: 2025/07/22 02:36:38 by ouelkhar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_word_start(char c)
{
	return (!f_isspace(c) && !is_operator(c) && !is_quote(c) && c != '$'); 
}

int	handle_word(t_token **token, char *input, t_lexvars *st)
{
	t_token_vars	vars;
	int				start;
	char			*part;

	start = st->i;
	while ((input[st->i] && is_word_start(input[st->i]))
		|| (input[st->i] == '$' && st->heredoc))
		st->i++;
	part = ft_substr(input, start, st->i - start);
	if (!part)
		return (0);
	vars.value = part;
	vars.type = WORD;
	vars.space = st->space;
	vars.quoted = false;
	add_token(token, &vars);
	free(part);
	st->space = f_isspace(input[st->i]);
	if (st->heredoc)
		st->heredoc = false;
	return (1);
}

t_token_type	get_opr_type(char *input, int i)
{
	if (input[i] == '>' && input[i + 1] == '>')
		return (APPEND);
	if (input[i] == '<' && input[i + 1] == '<')
		return (HEREDOC);
	if (input[i] == '>')
		return (REDIR_OUT);
	if (input[i] == '<')
		return (REDIR_IN);
	if (input[i] == '|')
		return (PIPE);
	return (0);
}

int	check_operator(t_oprvars *op_vars)
{
	t_token_type	type;
	t_token_vars	vars;

	type = get_opr_type(op_vars->input, op_vars->i);
	vars.type = type;
	vars.space = op_vars->space;
	vars.quoted = false;
	if (type == APPEND || type == HEREDOC)
	{
		if (type == HEREDOC)
			*(op_vars->heredoc) = true;
		vars.value = ft_substr(op_vars->input, op_vars->i, 2);
		add_token(op_vars->token, &vars);
		free(vars.value);
		return (op_vars->i + 2);
	}
	vars.value = ft_substr(op_vars->input, op_vars->i, 1);
	add_token(op_vars->token, &vars);
	free(vars.value);
	return (op_vars->i + 1);
}

int	handle_operator(t_token **token, char *input, int i, t_lexvars *st)
{
	t_oprvars	op_vars;

	op_vars.input = input;
	op_vars.i = i;
	op_vars.token = token;
	op_vars.space = st->space;
	op_vars.heredoc = &st->heredoc;
	return (check_operator(&op_vars));
}
