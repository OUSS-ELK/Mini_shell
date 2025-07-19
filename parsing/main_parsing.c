/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_parsing.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ouelkhar <ouelkhar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 19:33:32 by ouelkhar          #+#    #+#             */
/*   Updated: 2025/07/19 21:33:52 by ouelkhar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int handle_quote(t_token **token, char *input, t_env *env, t_lexvars *st)
{
	t_token_vars	vars;
	t_quotevars		qt_var;
	char			*part;
	int				len;

	qt_var.input = input;
	qt_var.start = st->i;
	qt_var.output = &part;
	qt_var.env = env;
	qt_var.heredoc = &st->heredoc;
	len = inside_quote(&qt_var);
	if (len == -1 || !part)
		return (0);
	vars.value = part;
	vars.type = WORD;
	vars.space = st->space;
	vars.quoted = true;
	add_token(token, &vars);
	free(vars.value);
	st->space = f_isspace(input[len]);
	st->i = len;
	if (st->heredoc)
		st->heredoc = false;
	return (1);
}

void	else_case(t_token **token, char *input, t_lexvars *st)
{
	t_token_vars	vars;

	// printf("Else Case\n");
	if (input[st->i] == '$' && input[st->i + 1] == '\0')
	{
		// printf("$ -> '0'\n");
		vars.value = "$";
		vars.type = WORD;
		vars.space = st->space;
		vars.quoted = false;
		add_token(token, &vars);
	}
	// if (input[st->i] == '\\')
	// 	st->i++;
	st->i++;
}

void	handle_space(char *input, t_lexvars *st)
{
	while (input[st->i] && f_isspace(input[st->i]))
		st->i++;
	st->space = true;
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
		if (!handle_expansion(token, input, env, st))
			return (0);
	}
	return (1);
}

int	lexing(t_token **token, char *input, t_env *env, t_lexvars *st)
{
	if (f_isspace(input[st->i]))
		handle_space(input, st);
	else if (is_quote(input[st->i]))
	{
		printf("Quote\n");
		if (!handle_quote(token, input, env, st))
			return (0);
	}
	else if (is_word_start(input[st->i]))
	{
		printf("Word\n");
		if (!handle_word(token, input, st))
			return (0);
	}
	else if (input[st->i] == '$' && input[st->i + 1] && !st->heredoc)
	{
		printf("Valide expand\n");
		printf("heredoc = %d\n", st->heredoc);
		if (input[st->i + 1] && !valid_expand(input[st->i], input[st->i + 1]))
		{
			if (!handle_invalide_expand(token, input, st))
				return (0);
		}
		else
		{
			if (!handle_expansion(token, input, env, st))
				return (0);
		}
		// if (!expansion(token, input, env, st))
		// 	return (0);
	}
	else if (is_operator(input[st->i]))
	{
		printf("Operator\n");
		st->i = handle_operator(token, input, st->i, st);
	}
	else
		else_case(token, input, st);
	return (1);
}

int lexer_input(t_token **token, char *input, t_env *env)
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
	if (st.i > 0)
		merge_words(token);
	return (1);
}
