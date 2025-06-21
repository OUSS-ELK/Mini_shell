
#include"execution.h"

// helper searches for a key in the env list and return strdup of its value
static char	*get_env_value_dup(const char *key, t_env *env_lst)
{
	while (env_lst)
	{
		if (ft_strcmp(env_lst->key, key) == 0)
			return (ft_strdup(env_lst->value));
		env_lst = env_lst->next;
	}
	return (NULL);
}

// fetch PATH var and split it into directories
static char **fetch_paths(t_env *env_lst)
{
	char	*line;
	char	**paths;

	line = get_env_value_dup("PATH", env_lst);
	if (!line)
		return (NULL);
	paths = ft_split(line, ':');
	free(line);
	return (paths);
}

// static bool	is_directory(const char *path) // TO LOOK UP LATER (THIS METHOD IS READ ONLY SINCE IT DOESNT OPEN AND U DONT NEED TO CLOSE)
// {
// 	struct sta	stat_buffer;

// 	if (stat(path, &stat_buffer) == 0)
// 		return (S_ISDIR(stat_buffer.st_mode));
// 	return (false);
// }

// core func helper that loops threough the PATH to locate the binary
static char	*search_path_in_env(char *cmd, t_exec *exec, char **paths)
{
	char	*full_path;
	int		i;

	i = 0;
	while (paths[i])
	{
		full_path = ft_strjoin(paths[i], "/", cmd);
		if (!full_path)
			break ;
		if (access(full_path, X_OK) == 0)
		{
			freestrarr(paths);
			return (full_path);
		}
		free(full_path);
	}
	freestrarr(paths);
	exec->exit_status = 127;
	return (NULL);
}

/*
	CORE FUNC: Resolves command name to full path using PATH.
	If cmd is already absolute or relative, it is returned as is.
	Else it searches through PATH
*/

char	*get_command_path(char *cmd, t_exec *exec)
{
	char	**paths;

	if (!cmd || !*cmd)
	{
		exec->exit_status = 127;
		return (NULL);
	}
	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, X_OK) == 0 && !is_directory(cmd))
			return (ft_strdup(cmd));
		if (is_directory(cmd))
			exec->exit_status = 126;
		else
			exec->exit_status = 127;
		return (NULL);
	}
	paths = fetch_paths(exec->env_lst);
	if (!paths)
	{
		exec->exit_status = 127;
		return (NULL);
	}
	return (search_path_in_env(cmd, exec, paths));
}

/*

	user types a cmd
	
	parser tokenizes - fills t_cmd struct and calls exec_cmd()

	exec cmd() calls:
		get_command_path(cmd->args[0], exec)

	[get_command_path()]
├── Checks if command contains '/'
│   ├── If yes → check if accessible and not a directory
│   └── If not accessible → set exit_status and return NULL
│
├── If not → fetches PATH list:
│   └── fetch_paths(exec->env_lst)
│        └── get_env_value_dup("PATH", env_lst)
│             └── returns strdup of PATH value
│        └── ft_split(PATH, ':') → ["/bin", "/usr/bin", ...]
│
├── Iterates PATHs:
│   └── For each path, ft_strjoin(path, "/", cmd)
│         └── e.g., "/bin/ls"
│         └── If access() returns X_OK → Success!
│             └── Return full path
│
└── If not found in any PATH dir → exit_status = 127, return NULL

*/