/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_quote.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ouelkhar <ouelkhar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 03:44:30 by ouelkhar          #+#    #+#             */
/*   Updated: 2025/07/23 03:56:15 by ouelkhar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*expand_simple_qt(t_exp_strvars *exp_var)
{
	char	*tmp;
	char	*value;
	int		start;

	start = exp_var->i;
	while (ft_isalnum(exp_var->str[exp_var->i]) || exp_var->str[exp_var->i] == '_')
		exp_var->i++;
	tmp = ft_substr(exp_var->str, start, exp_var->i - start);
	value = ft_getenv(tmp, exp_var->env);
	exp_var->result = f_strjoin_free(exp_var->result, value ? value : "");
	free(tmp);
	return (exp_var->result);
}

void	init_expand_var(t_exp_strvars *exp_var, char *str, t_env *env, bool heredoc, char *result)
{
	exp_var->str = str;
	exp_var->i = 0;
	exp_var->env = env;
	exp_var->result = result;
	exp_var->heredoc = heredoc;
}

void    handle_dollar_expansion(t_exp_strvars *exp_var)
{
    char    *tmp;
    char    *value;

	if (exp_var->heredoc)
		exp_var->i--;
	else if (exp_var->str[exp_var->i] == '?')
		exp_var->result = exit_status(exp_var->result, &exp_var->i);
	else if (ft_isalpha(exp_var->str[exp_var->i]) || exp_var->str[exp_var->i] == '_')
		exp_var->result = expand_simple_qt(exp_var);
	else if (ft_isdigit(exp_var->str[exp_var->i]))
	{
		tmp = ft_substr(exp_var->str, exp_var->i, 1);
		value = ft_getenv(tmp, exp_var->env);
		exp_var->result = f_strjoin_free(exp_var->result, value ? value : "");
		free(tmp);
		exp_var->i++;
	}
	else
		exp_var->result = f_strjoin_char(exp_var->result, '$');
}

char	*expand_loop(char *str, t_env *env, bool heredoc, char *result)
{
	t_exp_strvars	exp_var;

	init_expand_var(&exp_var, str, env, heredoc, result);
	while (exp_var.str[exp_var.i])
	{
		if (exp_var.str[exp_var.i] == '$')
		{
			exp_var.i++;
			handle_dollar_expansion(&exp_var);
		}
		else
			exp_var.result = f_strjoin_char(exp_var.result, exp_var.str[exp_var.i++]);
	}
	return (exp_var.result);
}

char	*expand_var_str(char *str, t_env *env, bool heredoc)
{
	char	*result;

	result = ft_strdup("");
	if (!result)
		return (NULL);
	return (expand_loop(str, env, heredoc, result));
}
