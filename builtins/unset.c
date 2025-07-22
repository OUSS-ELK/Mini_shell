/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bel-abde <bel-abde@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 13:39:15 by bel-abde          #+#    #+#             */
/*   Updated: 2025/07/21 09:02:30 by bel-abde         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	is_valid_identifier(const char *str)
{
	if (!str || !*str)
		return (0);
	if (!ft_isalpha(*str) && *str != '_')
		return (0);
	str++;
	while (*str)
	{
		if (!ft_isalnum(*str) && *str != '_')
			return (0);
		str++;
	}
	return (1);
}

static void	print_unset_error(char *arg)
{
	ft_putstr_fd("unset: `", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
}

static void	remove_env_by_key(t_env **env, char *key)
{
	t_env	*tmp;
	t_env	*to_remove;

	tmp = *env;
	while (tmp)
	{
		if (ft_strcmp(tmp->key, key) == 0)
		{
			to_remove = tmp;
			tmp = tmp->next;
			remove_env_node(env, to_remove);
			break ;
		}
		tmp = tmp->next;
	}
}

static int	unset_env_var(char *arg, t_env **env, int *ret)
{
	if (!is_valid_identifier(arg))
	{
		print_unset_error(arg);
		*ret = 1;
		return (0);
	}
	if (ft_strcmp(arg, "_") != 0)
		remove_env_by_key(env, arg);
	return (0);
}

int	ft_unset(char **cmd, t_env **env)
{
	int	ret;

	if (!cmd || !*cmd || !env || !*env)
		return (1);
	ret = 0;
	cmd++;
	while (*cmd)
	{
		unset_env_var(*cmd, env, &ret);
		cmd++;
	}
	return (ret);
}

// int ft_unset(char **cmd, t_env **env)
// {
//     t_env *tmp;
//     t_env *to_remove;
//     int ret;

//     if (!cmd || !*cmd || !env || !*env)
//         return (1);

//     ret = 0;
//     cmd++;
//     while (*cmd)
//     {
//         if (!is_valid_identifier(*cmd))
//         {
//             ft_putstr_fd("unset: `", 2);
//             ft_putstr_fd(*cmd, 2);
//             ft_putstr_fd("': not a valid identifier\n", 2);
//             ret = 1;
//         }
//         else if (ft_strcmp(*cmd, "_") != 0)
//         {
//             tmp = *env;
//             while (tmp)
//             {
//                 if (ft_strcmp(tmp->key, *cmd) == 0)
//                 {
//                     to_remove = tmp;
//                     tmp = tmp->next;
//                     remove_env_node(env, to_remove);
//                     break ;
//                 }
//                 tmp = tmp->next;
//             }
//         }
//         cmd++;
//     }
//     return (ret);
// }
