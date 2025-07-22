/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_parsing.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ouelkhar <ouelkhar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 19:33:32 by ouelkhar          #+#    #+#             */
/*   Updated: 2025/07/22 02:33:48 by ouelkhar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	else_case(t_token **token, char *input, t_lexvars *st)
{
	t_token_vars	vars;

	if (input[st->i] == '$' && input[st->i + 1] == '\0')
	{
		vars.value = "$";
		vars.type = WORD;
		vars.space = st->space;
		vars.quoted = false;
		add_token(token, &vars);
	}
	st->i++;
}

void	handle_space(char *input, t_lexvars *st)
{
	while (input[st->i] && f_isspace(input[st->i]))
		st->i++;
	st->space = true;
}

int	lexing(t_token **token, char *input, t_env *env, t_lexvars *st)
{
	if (f_isspace(input[st->i]))
		handle_space(input, st);
	else if (is_quote(input[st->i]))
	{
		if (!handle_quote(token, input, env, st))
			return (0);
	}
	else if (input[st->i] == '$' && input[st->i + 1] && !st->heredoc)
	{
		if (!expansion(token, input, env, st))
			return (0);
	}
	else if (is_word_start(input[st->i])
		|| (input[st->i] == '$' && st->heredoc))
	{
		if (!handle_word(token, input, st))
			return (0);
	}
	else if (is_operator(input[st->i]))
		st->i = handle_operator(token, input, st->i, st);
	else
		else_case(token, input, st);
	return (1);
}

int	lexer_input(t_token **token, char *input, t_env *env)
{
	t_lexvars	st;

	st.i = 0;
	st.space = false;
	st.heredoc = false;
	while (input[st.i])
	{
		if (!lexing(token, input, env, &st))
			return (0);
	}
	if (*token && is_mergeable_words(*token))
		merge_words(token);
	return (1);
}

int	parsing_function(t_token **token, char *input, t_cmd **cmd, t_env **envr)
{
	if (!check_quote(input))
	{
		write_error(3);
		return (0);
	}
	if (!lexer_input(token, input, *envr))
	{
		write_error(2);
		return (0);
	}
	*cmd = parse_cmd(token);
	if (!*cmd)
		return (0);
	return (1);
}
