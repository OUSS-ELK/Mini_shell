/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bel-abde <bel-abde@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 10:18:33 by bel-abde          #+#    #+#             */
/*   Updated: 2025/07/26 08:47:01 by bel-abde         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	export_add_to_env(t_env **env, char ***splitted, char *has_value)
{
	if (!has_value)
	{
		free((*splitted)[1]);
		(*splitted)[1] = NULL;
	}
	export_add_node(env, *splitted);
	free(*splitted);
	*splitted = NULL;
}

void	export_append(t_env *var, char ***splitted)
{
	char	*new_val;

	new_val = ft_strjoin(var->value, (*splitted)[1]);
	free(var->value);
	var->value = new_val;
	free((*splitted)[0]);
	free((*splitted)[1]);
	free(*splitted);
	*splitted = NULL;
}

void	export_print(t_env *env)
{
	while (env)
	{
		printf("declare -x %s", env->key);
		if (env->value)
			printf("=\"%s\"", env->value);
		printf("\n");
		env = env->next;
	}
}

int	handle_export_logic(char **splitted, t_exec *exec, char *arg)
{
	t_env	*var;

	var = exec->env_lst;
	while (var && ft_strcmp(var->key, splitted[0]) != 0)
		var = var->next;
	if (!var)
	{
		if (splitted[1] == NULL)
		{
			free(splitted[0]);
			free(splitted);
			return (1);
		}
		if (!export_add_node(&(exec->env_lst), splitted))
		{
			free(splitted[0]);
			free(splitted[1]);
			return (free(splitted), 0);
		}
	}
	else if (ft_strchr(arg, '=') && *(ft_strchr(arg, '=') - 1) == '+')
		export_append(var, &splitted);
	else
		export_replace(var, &splitted, ft_strchr(arg, '='));
	return (1);
}

int	ft_export(char **av, t_exec *exec)
{
	int		i;
	int		error;
	char	**splitted;

	if (!av[1])
		return (export_print(exec->env_lst), 0);
	error = 0;
	i = 0;
	while (av[++i])
	{
		if (!export_valid_identifier(av[i]))
		{
			error = 1;
			continue ;
		}
		splitted = split_export_arg(av[i]);
		if (!splitted || !handle_export_logic(splitted, exec, av[i])
			|| !splitted[1])
			error = 1;
	}
	return (error);
}
