/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ouelkhar <ouelkhar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 19:33:50 by ouelkhar          #+#    #+#             */
/*   Updated: 2025/07/22 02:44:17 by ouelkhar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	expand_exit_status(t_token **token, bool *space)
{
	t_token_vars	vars;
	char			*expanded;

	expanded = ft_itoa(g_exit_status);
	if (!expanded)
		return (-1);
	vars.value = expanded;
	vars.type = WORD;
	vars.space = *space;
	vars.quoted = false;
	add_token(token, &vars);
	*space = false;
	free(expanded);
	return (0);
}

void	grap_key(char *input, int start, int *len)
{
	*len = 0;
	if (ft_isdigit(input[start]))
		*len = 1;
	else
	{
		while (input[start + *len] && (ft_isalnum(input[start + *len])
				|| input[start + *len] == '_'))
			(*len)++;
	}
}

int	expand_simple_var(t_spl_exp_vars *var)
{
	t_token_vars	vars;
	int				len;
	char			*var_name;
	char			*expanded;

	grap_key(var->input, var->start, &len);
	var_name = ft_substr(var->input, var->start, len);
	if (!var_name)
		return (-1);
	expanded = ft_getenv(var_name, var->env);
	free(var_name);
	if (!expanded || expanded[0] == '\0')
		return (var->start + len - 1);
	vars.value = expanded;
	vars.type = WORD;
	vars.space = *(var)->space;
	vars.quoted = false;
	add_token(var->token, &vars);
	*(var)->space = false;
	return (var->start + len - 1);
}

int	expanding_var(t_expndvars *exp_var)
{
	int				start;
	int				ret;
	t_spl_exp_vars	var;

	start = exp_var->i + 1;
	var.token = exp_var->token;
	var.input = exp_var->input;
	var.start = start;
	var.env = exp_var->env;
	var.space = exp_var->space;
	if (exp_var->input[start] == '?')
	{
		ret = expand_exit_status(exp_var->token, exp_var->space);
		if (ret == -1)
			return (-1);
		return (start + 1);
	}
	ret = expand_simple_var(&var);
	if (ret == -1)
		return (-1);
	return (ret + 1);
}
