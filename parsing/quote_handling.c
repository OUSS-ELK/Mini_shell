/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ouelkhar <ouelkhar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 03:47:01 by ouelkhar          #+#    #+#             */
/*   Updated: 2025/07/23 10:03:28 by ouelkhar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_quote(char *input)
{
	int	i;
	int	sq;
	int	dq;

	i = 0;
	sq = 0;
	dq = 0;
	while (input[i])
	{
		if (input[i] == '\'' && !dq)
			sq = !sq;
		else if (input[i] == '"' && !sq)
			dq = !dq;
		i++;
	}
	if (sq || dq)
		return (0);
	return (1);
}

char	*handle_qte_exp(char *str, char quote, t_env *env, bool heredoc)
{
	char	*expand;

	if (heredoc || quote == '\'')
		expand = ft_strdup(str);
	else
		expand = expand_var_str(str, env, heredoc);
	return (expand);
}

char	*extract_quoted_content(char *input, int start, int *end)
{
	int		i;
	char	*str;
	char	quote;

	quote = input[start];
	i = start + 1;
	while (input[i] && input[i] != quote)
		i++;
	if (input[i] != quote)
		return (NULL);
	str = ft_substr(input, start + 1, i - start - 1);
	if (!str)
		return (NULL);
	*end = i;
	return (str);
}

int	inside_quote(t_quotevars *qt_var)
{
	char	*str;
	char	*expand;
	int		end;
	char	quote;

	quote = qt_var->input[qt_var->start];
	str = extract_quoted_content(qt_var->input, qt_var->start, &end);
	if (!str)
		return (-1);
	expand = handle_qte_exp(str, quote, qt_var->env, *(qt_var->heredoc));
	free(str);
	if (!expand)
		return (-1);
	*(qt_var->output) = expand;
	return (end + 1);
}

int	handle_quote(t_token **token, char *input, t_env *env, t_lexvars *st)
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

// int	check_quote(char *input)
// {
// 	int	i;
// 	int	sq;
// 	int	dq;

// 	i = 0;
// 	sq = 0;
// 	dq = 0;
// 	while (input[i])
// 	{
// 		if (input[i] == '\'' && !dq)
// 			sq = !sq;
// 		else if (input[i] == '"' && !sq)
// 			dq = !dq;
// 		i++;
// 	}
// 	if (sq || dq)
// 		return (0);
// 	return (1);
// }

// char	*handle_quote_expansion(char *str, char quote, t_env *env, bool heredoc)
// {
// 	char	*expand;

// 	if (heredoc || quote == '\'')
// 		expand = ft_strdup(str);
// 	else
// 		expand = expand_var_str(str, env, heredoc);
// 	return (expand);
// }

// char	*extract_quoted_content(char *input, int start, int *end)
// {
// 	int		i;
// 	char	*str;
// 	char	quote;

// 	quote = input[start];
// 	i = start + 1;
// 	while (input[i] && input[i] != quote)
// 		i++;
// 	if (input[i] != quote)
// 		return (NULL);
// 	str = ft_substr(input, start + 1, i - start - 1);
// 	if (!str)
// 		return (NULL);
// 	*end = i;
// 	return (str);
// }

// int inside_quote(t_quotevars *qt_var)
// {
// 	char	*str;
// 	char	*expand;
// 	int		end;
// 	char	quote;

// 	quote = qt_var->input[qt_var->start];
// 	str = extract_quoted_content(qt_var->input, qt_var->start, &end);
// 	if (!str)
// 		return (-1);
// 	expand = handle_quote_expansion(str, quote, qt_var->env,
			// *(qt_var->heredoc));
// 	free(str);
// 	if (!expand)
// 		return (-1);
// 	*(qt_var->output) = expand;
// 	return (end + 1);
// }

// int handle_quote(t_token **token, char *input, t_env *env, t_lexvars *st)
// {
// 	t_token_vars	vars;
// 	t_quotevars		qt_var;
// 	char			*part;
// 	int				len;

// 	qt_var.input = input;
// 	qt_var.start = st->i;
// 	qt_var.output = &part;
// 	qt_var.env = env;
// 	qt_var.heredoc = &st->heredoc;
// 	len = inside_quote(&qt_var);
// 	if (len == -1 || !part)
// 		return (0);
// 	vars.value = part;
// 	vars.type = WORD;
// 	vars.space = st->space;
// 	vars.quoted = true;
// 	add_token(token, &vars);
// 	free(vars.value);
// 	st->space = f_isspace(input[len]);
// 	st->i = len;
// 	if (st->heredoc)
// 		st->heredoc = false;
// 	return (1);
// }
