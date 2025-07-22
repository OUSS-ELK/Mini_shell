/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bel-abde <bel-abde@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 08:56:13 by bel-abde          #+#    #+#             */
/*   Updated: 2025/07/21 09:01:41 by bel-abde         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*pwd_getenv(char *key, t_env *env)
{
	while (env)
	{
		if (env->key && env->value && strcmp(env->key, key) == 0)
			return (strdup(env->value));
		env = env->next;
	}
	return (NULL);
}

int	ft_pwd(t_env **env)
{
	char	*cwd;

	if (!env || !*env)
	{
		perror("pwd");
		g_exit_status = 1;
		return (0);
	}
	cwd = pwd_getenv("PWD", *env);
	if (!cwd)
		cwd = getcwd(NULL, 0);
	if (cwd != NULL)
	{
		printf("%s\n", cwd);
		free(cwd);
		g_exit_status = 0;
	}
	else
	{
		perror("pwd");
		g_exit_status = 1;
	}
	return (0);
}
