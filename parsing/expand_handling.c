/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_handling.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ouelkhar <ouelkhar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 02:31:50 by ouelkhar          #+#    #+#             */
/*   Updated: 2025/07/22 02:51:06 by ouelkhar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	handl_valid_expnd(t_token **token, char *input, t_env *env, t_lexvars *st)
{
	t_expndvars	exp_var;
	int			new_i;

	exp_var.token = token;
	exp_var.i = st->i;
	exp_var.input = input;
	exp_var.env = env;
	exp_var.space = &st->space;
	exp_var.heredoc = st->heredoc;
	new_i = expanding_var(&exp_var);
	if (new_i == -1)
		return (0);
	st->i = new_i;
	return (1);
}

int	handle_invalide_expand(t_token **token, char *input, t_lexvars *st)
{
	t_token_vars	vars;
	int				start;
	int				len;
	char			*part;

	start = st->i;
	len = 1;
	while (input[st->i + len] && is_word_start(input[st->i + len]))
		len++;
	part = ft_substr(input, start, len);
	if (!part)
		return (0);
	vars.value = part;
	vars.type = WORD;
	vars.space = st->space;
	vars.quoted = false;
	add_token(token, &vars);
	free(part);
	st->i += len;
	st->space = f_isspace(input[st->i]);
	if (st->heredoc)
		st->heredoc = false;
	return (1);
}

int	valid_expand(char input, char next)
{
	return (input == '$'
		&& (ft_isalnum(next)
			|| next == '_'
			|| next == '?'
			|| next == '"'
			|| next == '\''));
}

int	expansion(t_token **token, char *input, t_env *env, t_lexvars *st)
{
	if (input[st->i + 1] && !valid_expand(input[st->i], input[st->i + 1]))
	{
		if (!handle_invalide_expand(token, input, st))
			return (0);
	}
	else
	{
		if (!handl_valid_expnd(token, input, env, st))
			return (0);
	}
	return (1);
}
