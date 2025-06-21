
#include "execution.h"


// called after heredoc content is read, it expands $VAR in heredoc
bool	expander(t_exec *exec, t_redir *redir)
{
	int		fd[2];
	char	*line;
	char	*expanded;

	if (pipe(fd) < 0)
		return (perror("pipe"), true);
	while ((line = get_next_line(redir->fd[0])))
	{
		expanded = expand_line(line, exec->env_lst);
		free(line);
		if (!expanded)
		{
			close(fd[0]);
			close(fd[1]);
			return (true);
		}
		ft_putstr_fd(expanded, fd[1]);
		free(expanded);
	}
	close(fd[1]);
	close(redir->fd[0]);
	redir->fd[0] = fd[0];
	return (false);
}


// search for $var in the line
// replace it with the corresponding value from env_lst
// handle edge cases like $?, undefined variables and $VAR123extra
// Expand $VAR from line using env list
char	*expand_line(const char *line, t_env *env_lst)
{
	char	*result;
	char	*tmp;
	int		i = 0, start;
	char	*var_name;
	char	*var_value;

	if (!line)
		return (NULL);
	result = ft_strdup("");
	while (line[i])
	{
		if (line[i] == '$' && line[i + 1] && (ft_isalnum(line[i + 1]) || line[i + 1] == '_'))
		{
			start = ++i;
			while (line[i] && (ft_isalnum(line[i]) || line[i] == '_'))
				i++;
			var_name = ft_substr(line, start, i - start);
			var_value = get_env_value(var_name, env_lst);
			tmp = result;
			result = ft_strjoin(result, var_value ? var_value : "", NULL);
			free(tmp);
			free(var_name);
		}
		else
		{
			start = i;
			while (line[i] && line[i] != '$')
				i++;
			tmp = ft_substr(line, start, i - start);
			char *old_result = result;
			result = ft_strjoin(result, tmp, NULL);
			free(old_result);
			free(tmp);
		}
	}
	return (result);
}

// fetch the value for a var name from the env list
// Helper to fetch env value from key
char	*get_env_value(const char *key, t_env *env_lst)
{
	while (env_lst)
	{
		if (ft_strcmp(env_lst->key, key) == 0)
			return (env_lst->value);
		env_lst = env_lst->next;
	}
	return (NULL);
}
