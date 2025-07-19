#include "minishell.h"

void	custom_error(char *cmd, char *error, int status)
{
	write(2, "mnshell: ", 9);
	if (cmd)
	{
		write(2, cmd, ft_strlen(cmd));
		write(2, ": ", 2);
	}
	write(2, error, ft_strlen(error));
	// printf("Before exit\n");
	exit(status);
}

void	ft_free_env_list(t_env **list)
{
	t_env	*current;
	t_env	*next;

	if (!list || !*list)
		return ;
	current = *list;
	while (current)
	{
		next = current->next;
		free(current->key);
		free(current->value);
		free(current);
		current = next;
	}
	*list = NULL;
}

/* ************************************************************************** */
/*  env_to_array.c                                                            */
/* ************************************************************************** */


/*
 * Count how many elements are in the env linked list.
 */
static size_t	env_count(t_env *head)
{
	size_t	n = 0;

	while (head)
	{
		n++;
		head = head->next;
	}
	return (n);
}

/*
 * Join key + '=' + value (value may be NULL).
 * Returns a freshly‑allocated "key=val" C‑string or NULL on malloc failure.
 * If value is NULL, we still create "key=" (this is what the real env does).
 */
static char		*join_env_kv(t_env *node)
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
		joined = ft_strdup(tmp);           /* keep the trailing '='        */
	free(tmp);
	return (joined);
}

/*
 * Convert our t_env linked list into a classic **envp array.
 * The array is NULL‑terminated so it can be passed directly to execve().
 * On any allocation failure the function frees what it already built
 * and returns NULL, leaving g_exit_status set to 1.
 */
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
			/* clean up partially‑filled array */
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


void	setup_heredoc_sig(void)
{
	signal(SIGINT, handle_heredoc_sigint);
	signal(SIGQUIT, SIG_IGN);
}

void	restore_sigs(void)
{
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
}

void	setup_parent_heredoc_sigs(void)
{
	signal(SIGINT, handle_parent_heredoc_sigint);
	signal(SIGQUIT, SIG_IGN);
}

void	setup_parent_waiting_sigs(void)
{
	signal(SIGINT, handle_parent_sigint);
	signal(SIGQUIT, SIG_IGN);
}

void	setup_child_sigs(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

void	handle_sigint(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	rl_replace_line("", 0); // clears the current line
	rl_on_new_line(); // moves to new line
	rl_redisplay(); // redisplay the prompt
	g_exit_status = 1;
}

void	handle_parent_sigint(int sig)
{
	(void)sig;
	g_exit_status = 130; // means process was terminated by SIGINT = signal 2
}

void	handle_parent_heredoc_sigint(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	g_exit_status = 1;
}

void	handle_heredoc_sigint(int sig)
{
	(void)sig;
	exit(1);
}

void    handle_signals(void)
{
        signal(SIGINT, handle_sigint);
        signal(SIGQUIT, SIG_IGN);
}


// static void	ft_free_strarr(char **arr)
// {
// 	int	i;

// 	if (!arr)
// 		return;
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
