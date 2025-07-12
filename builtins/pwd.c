
#include "../minishell.h"

/*
 * Searches for the key in the env linked list and returns a copy of the value.
 * Caller must free the returned value.
 */
static char	*pwd_getenv(char *key, t_env *env)
{
	while (env)
	{
		if (env->key && env->value && strcmp(env->key, key) == 0)
			return (strdup(env->value)); // return a **copy** so it can be freed
		env = env->next;
	}
	return (NULL);
}

/*
 * Prints the current working directory to stdout.
 * Looks for "PWD" in the environment list. If not found,
 * falls back to using getcwd().
 *
 * @param env: pointer to t_env list
 * @return: always returns 0
 * Side effects: prints to stdout or stderr, sets g_exit_status
 */
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
