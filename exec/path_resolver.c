#include "../minishell.h"

/*
 * Retrieves the value of the PATH environment variable from the linked list.
 * Returns the value (char *) if found, otherwise returns NULL.
 */
char	*get_path_value_from_env(t_env *env)
{
	while (env)
	{
		if (!ft_strncmp(env->key, "PATH", 4) && env->key[4] == '\0')
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

/*
 * Searches for the given command in the directories listed in path_dirs.
 * If an executable version of the command is found, stores its full path
 * in full_path and returns. Otherwise, sets full_path to NULL.
 */
void	search_command_in_path_dirs(char **full_path, char **path_dirs, char *cmd)
{
	char	*tmp;

	*full_path = NULL;
	while (*path_dirs)
	{
		tmp = ft_strjoin(*path_dirs, "/");
		if (!tmp)
			return ;
		*full_path = ft_strjoin(tmp, cmd);
		free(tmp);
		if (!*full_path)
			return ;
		if (access(*full_path, X_OK) == 0)
			return ;
		free(*full_path);
		*full_path = NULL;
		path_dirs++;
	}
}

/*
 * Attempts to find the given command in the current working directory.
 * If found and executable, returns the full path to the command.
 * Otherwise returns NULL.
 */
char	*search_command_in_cwd(char *cmd)
{
	char	*cwd;
	char	*tmp;
	char	*result;

	cwd = getcwd(NULL, 0);
	if (!cwd)
		return (NULL);
	tmp = ft_strjoin(cwd, "/");
	free(cwd);
	if (!tmp)
		return (NULL);
	result = ft_strjoin(tmp, cmd);
	free(tmp);
	if (!result)
		return (NULL);
	if (access(result, X_OK) == 0)
		return (result);
	free(result);
	return (NULL);
}

/*
 * Resolves the full path to an executable command.
 * 1. If the command is a direct path (starts with '.' or '/'), checks it.
 * 2. If PATH is present in the environment, tries all PATH directories.
 * 3. If not found, tries the current working directory as fallback.
 * 
 * If no path is found and PATH is unset, sets *no_file = 1.
 */
char	*resolve_path(char *cmd, t_env *env, int *no_file)
{
	char	*full_path;
	char	*path_value;
	char	**path_dirs;
	char	*cwd_exec;

	*no_file = 0;
	full_path = check_if_direct_path_valid(cmd);
	if (full_path)
		return (full_path);
	path_value = get_path_value_from_env(env);
	if (path_value)
	{
		path_dirs = split_t(path_value, ':');
		if (!path_dirs)
			return (NULL);
		search_command_in_path_dirs(&full_path, path_dirs, cmd);
		ft_free_array(path_dirs);
		if (full_path)
			return (full_path);
	}
	else
		*no_file = 1;
	cwd_exec = search_command_in_cwd(cmd);
	if (cwd_exec)
		return (cwd_exec);
	return (NULL);
}

/*
 * Checks if the given path is a valid full or relative path to an executable.
 * If so, returns a duplicate of the path string. Otherwise, returns NULL.
 */
char	*check_if_direct_path_valid(char *path)
{
	if (!path)
		return (NULL);
	if (*path == '/' || *path == '.')
	{
		if (access(path, X_OK) == 0)
			return (ft_strdup(path));
		else
			return (NULL);
	}
	return (NULL);
}
