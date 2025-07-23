/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bel-abde <bel-abde@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 02:27:27 by bel-abde          #+#    #+#             */
/*   Updated: 2025/07/23 02:29:03 by bel-abde         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static size_t	env_count(t_env *head)
{
	size_t	n;

	n = 0;
	while (head)
	{
		n++;
		head = head->next;
	}
	return (n);
}

static char	*join_env_kv(t_env *node)
{
	char	*tmp;
	char	*joined;

	if (!node)
		return (NULL);
	tmp = ft_strjoin(node->key, "=");
	if (!tmp)
		return (NULL);
	if (node->value)
		joined = ft_strjoin(tmp, node->value);
	else
		joined = ft_strdup(tmp);
	free(tmp);
	return (joined);
}

char	**env_to_array(t_env *env_head)
{
	size_t	i;
	size_t	cnt;
	char	**envp;

	cnt = env_count(env_head);
	envp = (char **)malloc(sizeof(char *) * (cnt + 1));
	if (!envp)
		return (g_exit_status = 1, NULL);
	i = 0;
	while (env_head)
	{
		envp[i] = join_env_kv(env_head);
		if (!envp[i])
		{
			while (i > 0)
				free(envp[--i]);
			free(envp);
			g_exit_status = 1;
			return (NULL);
		}
		i++;
		env_head = env_head->next;
	}
	envp[i] = NULL;
	return (envp);
}

// static void	ft_free_strarr(char **arr)
// {
// 	int	i;

// 	if (!arr)
// 		return ;
// 	i = 0;
// 	while (arr[i])
// 	{
// 		free(arr[i]);
// 		i++;
// 	}
// 	free(arr);
// }

// char	**env_to_array(t_env *lst)
// {
// 	size_t	len = 0;
// 	char	**arr;
// 	size_t	i = 0;

// 	for (t_env *tmp = lst; tmp; tmp = tmp->next)
// 		len++;

// 	arr = malloc(sizeof(char *) * (len + 1));
// 	if (!arr)
// 		return (NULL);
// 	while (lst)
// 	{
// 		size_t key_len = strlen(lst->key);
// 		size_t val_len = lst->value ? strlen(lst->value) : 0;

// 		arr[i] = malloc(key_len + 1 + val_len + 1);    /* KEY=VALUE\0 */
// 		if (!arr[i])
// 			return (ft_free_strarr(arr), NULL);
// 		memcpy(arr[i], lst->key, key_len);
// 		arr[i][key_len] = '=';
// 		if (lst->value)
// 			memcpy(arr[i] + key_len + 1, lst->value, val_len);
// 		arr[i][key_len + 1 + val_len] = '\0';
// 		i++;
// 		lst = lst->next;
// 	}
// 	arr[i] = NULL;
// 	return (arr);
// }
