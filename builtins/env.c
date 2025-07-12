#include "minishell.h"


/*
 * Implements the env builtin command.
 * Prints all environment variables in the format KEY=VALUE.
 * Special case: Updates "_" to "/usr/bin/env" if it exists and is printable.
 *
 * @param env: Pointer to your environment variable list (t_env **)
 * @return: Always returns 1. Sets g_exit_status accordingly.
 */
int	ft_env(t_env **env)
{
	t_env	*curr;

	if (!env || !*env)
	{
		g_exit_status = 1;
		return (1);
	}
	curr = *env;
	while (curr)
	{
		if (curr->key && strcmp(curr->key, "_") == 0 && curr->value)
		{
			free(curr->value);
			curr->value = strdup("/usr/bin/env");
			if (!curr->value)
				return (g_exit_status = 1, 1);
		}
		if (curr->key && curr->value)
			printf("%s=%s\n", curr->key, curr->value);
		curr = curr->next;
	}
	g_exit_status = 0;
	return (1);
}
