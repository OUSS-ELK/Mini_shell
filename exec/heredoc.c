#include "../minishell.h"


static t_redir	*get_last_heredoc(t_redir *r)
{
	t_redir	*last;

	last = NULL;
	while (r)
	{
		if (r->type == HEREDOC)
			last = r;
		r = r->next;
	}
	return (last);
}

// interate over every heredoc in every cmd
int	check_heredocs(t_cmd *first_cmd, t_env *env_lst)
{
	t_cmd	*cmd;
	t_redir	*r;
	t_redir	*last_heredoc;

	g_exit_status = 0;
	cmd = first_cmd;
	if (!cmd)
		return (1);
	while (cmd)
	{
		last_heredoc = get_last_heredoc(cmd->redir);
		setup_parent_heredoc_sigs();
		r = cmd->redir;
		while (r)
		{
			if (r->type == HEREDOC)
			{
				if (handle_one_heredoc(r, env_lst, (r == last_heredoc)) == -1
					|| g_exit_status == 1)
					return (0);
			}
			r = r->next;
		}
		restore_sigs();
		cmd = cmd->next;
	}
	// printf("Heredoc test\n");
	return (1);
}


// Handle one heredoc redir
int	handle_one_heredoc(t_redir *r, t_env *env, bool last)
{
	int	pipefd[2];
	pid_t	pid;
	if (r == NULL || r->type != HEREDOC)
		return (1);
	setup_parent_heredoc_sigs();
	if (pipe(pipefd) == -1)
		return (custom_error(NULL, "pipe fail\n", 1), -1);
	pid = fork();
	if (pid == -1)
		return (custom_error(NULL, "fork fail\n", 1), -1);
	if (pid == 0)
		open_heredoc_child(r, pipefd, env, last);
	else
	{
		parent_heredoc(pipefd, pid, r, last);
		restore_sigs();
	}
	if (g_exit_status == 0)
		return (1);
	else
		return (-1);
}


// // wait for child, keep read end if child succeeds (exit 0)
void	parent_heredoc(int *pipefd, pid_t pid, t_redir *r, bool last)
{
	int	status;
	sig_t	old_handler;

	close(pipefd[1]);                         /* parent only reads          */
	old_handler = signal(SIGINT, SIG_IGN);
	waitpid(pid, &status, 0);
	signal(SIGINT, old_handler);
	g_exit_status = WEXITSTATUS(status);
	if (g_exit_status == 0 && last)
		r->fd[0] = pipefd[0];                 /* store read end in redir    */
	else
	{
		close(pipefd[0]);                    /* error → close & mark -1    */
		r->fd[0] = -1;
	}
}
// void	parent_heredoc(int *pipefd, pid_t pid, t_redir *r, bool last)
// {
// 	int	status;
// 	sig_t	old_handler;
// 	char	buf[1024];


// 	old_handler = signal(SIGINT, SIG_IGN);
// 	waitpid(pid, &status, 0);
// 	signal(SIGINT, old_handler);
// 	g_exit_status = WEXITSTATUS(status);
// 	close(pipefd[1]);                         /* parent only reads          */
// 	if (g_exit_status == 0 && last)
// 	{
// 		r->fd[0] = pipefd[0];                 /* store read end in redir    */
// 			int		nbytes = read(r->fd[0], buf, sizeof(buf) - 1);
// 		printf("bytes value: %d\n", nbytes);
// 		printf("r fd not closed\n");
// 					while (nbytes)
// 			{
// 				printf("bytes value: %d\n", nbytes);
// 				buf[nbytes] = '\0';
// 				write(STDERR_FILENO, buf, nbytes);
// 				nbytes = read(r->fd[0], buf, sizeof(buf) - 1);
// 			}
// 		printf("while end\n");
// 	}
// 	else
// 	{
// 		printf("fd closed");
// 		close(pipefd[0]);                    /* error → close & mark -1    */
// 		r->fd[0] = -1;
// 	}
// }

//  CHILD: read stdin until delimiter, write to pipe, exit
int	open_heredoc_child(t_redir *heredoc, int *pipefd, t_env *env, bool last)
{
	char	*delim;

	close(pipefd[0]);                       /* child only writes          */
	setup_heredoc_sig();
	delim = NULL;
	delim = delim_join(heredoc->filename, "\n");
	if (!delim)
		custom_error(NULL, "heredoc delim fail\n", 1);
	ft_read_line(delim, pipefd, heredoc, env, last);
	free(delim);
	exit(0);
}

char	*delim_join(char *s1, char *s2)
{
	char	*new_str;
	size_t	total_len;
	size_t	i;
	size_t	j;

	if (!s1)
		return (ft_strdup(s2));
	if (!s2)
		return (ft_strdup(s1));
	total_len = ft_strlen(s1) + ft_strlen(s2) + 1;
	new_str = (char *)malloc(total_len * sizeof(char));
	if (!new_str)
	{
		free(s1);
		return (NULL);
	}
	i = -1;
	while (s1[++i])
		new_str[i] = s1[i];
	j = -1;
	while (s2[++j])
		new_str[i++] = s2[j];
	new_str[i] = '\0';
	free(s1);
	return (new_str);
}
