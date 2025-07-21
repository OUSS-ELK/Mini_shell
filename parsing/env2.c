/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env2.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oussama-elk <oussama-elk@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 19:33:50 by ouelkhar          #+#    #+#             */
/*   Updated: 2025/07/21 02:47:48 by oussama-elk      ###   ########.fr       */
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
	printf("exit status[%s]\n", expanded);
	*space = false;
	free(expanded);
	return (0);
}

int	expand_simple_var(t_token **token, char *input, int start, t_env *env, bool *space)
{
	t_token_vars	vars;
	int				len;
	char			*var_name;
	char			*expanded;

	len = 0;
	if (ft_isdigit(input[start]))
		len = 1;
	else
	{
		while (input[start + len] && (ft_isalnum(input[start + len])
				|| input[start + len] == '_'))
			len++;
	}
	var_name = ft_substr(input, start, len);
	// printf("inside expand simple var\n");
	if (!var_name)
		return (-1);
	expanded = ft_getenv(var_name, env);
	free(var_name);
	if (!expanded || expanded[0] == '\0')
		return (start + len - 1);
	vars.value = expanded;
	vars.type = WORD;
	vars.space = *space;
	vars.quoted = false;
	// printf("Valide expanded [%s]\n", expanded);
	add_token(token, &vars);
	*space = false;
	return (start + len - 1);
}

int expanding_var(t_expndvars *exp_var)
{
	int	start;
	int	ret;

	start = exp_var->i + 1;
	if (exp_var->input[start] == '?')
	{
		ret = expand_exit_status(exp_var->token,exp_var->space);
		if (ret == -1)
			return (-1);
		return (start + 1);
	}
	ret = expand_simple_var(exp_var->token,exp_var->input, start, exp_var->env,exp_var->space);
	if (ret == -1)
		return (-1);
	return (ret + 1);
}
