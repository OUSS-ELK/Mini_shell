#include "minishell.h"

int	g_exit_status = 0;

int parsing_function(t_token **token, char *input, char **env, t_cmd **cmd, t_env **envr)
{
	*envr = collect_env(env); 																			// 	collecte environement variables
	if (!*envr)
		return (0);																						//	return 0 for error & 1 for success 
	if (!check_quote(input))
	{
		write_error(2);
		return (0);
	}
	if (!lexer_input(token, input, *envr))
	{
		write_error(3);
		return (0);
	}
	*cmd = parse_cmd(token);
	if (!*cmd)
		return (0);
	return (1);
}


int main(int argc, char **argv, char **env)
{
	t_token *token;
	char	*input;
	t_cmd	*cmd;
	t_exec	exec;
	t_env	*envr;
        int original_stdin = dup(STDIN_FILENO);
        int original_stdout = dup(STDOUT_FILENO);
	(void)argc;
	(void)argv;
	envr = NULL;
	// atexit(ll);
	handle_signals();
	while (1)
	{
		input = readline(BOLDRED "[MINI_SHELL]$> " RESET);
		if (!input)
		{
			printf("Input error");															// Ctrl + D (EOF) 
			break ;
		}
		add_history(input);
		if (all_space(input))														// Empty or spaces
		{
			free(input);
			continue ;
		}
		token = NULL;
		cmd = NULL;
		// printf(BOLDCYAN "INPUT BY READLINE [%s]\n" RESET, input);
		if (!parsing_function(&token, input, env, &cmd, &envr))
		{
			write_error(1);
			cleanup(token, cmd, input, envr);
			continue ;
		}
		exec.env_lst = envr;
		// print_env(exec.env_lst);
		printf("DEBUG: parsing_function returned success\n");
		print_cmds(cmd);

		/* 2) HEREDOC ---------------------------------------------------- */
		if (!check_heredocs(cmd, envr))
		{
			printf("Error heredoc\n");
			cleanup(token, cmd, input, envr);
			continue;              /* back to prompt */
		}
		// printf("Heredoc chck finished\n");
// 		t_redir *r = cmd->redir;
// while (r)
// {
// 	if (r->type == HEREDOC)
// 		fprintf(stderr, "DEBUG parent: r->fd[0] = %d\n", r->fd[0]);
// 	r = r->next;
// }
	// print_heredoc_content_main(cmd);
		// 3) SINGLE BUILTIN ? ------------------------------------------ */
		exec.is_pipe = (cmd && cmd->next);      /*  true if pipeline */
		// printf("DEBUG: Checking for builtins...\n");
		if (builtin_check_execute(cmd, &exec, &envr) == 1)
		{
			cleanup(token, cmd, input, envr);
			continue;              /* builtin handled */
		}
		printf("DEBUG: Running execution_main\n");

		/* 4) PIPELINE / EXTERNAL --------------------------------------- */
		if (!execution_main(&exec, cmd, envr))
		{
			write_error(1);        /* fatal fork/pipe error */
		}
		cleanup(token, cmd, input, envr);
		dup2(original_stdin, STDIN_FILENO);
        dup2(original_stdout, STDOUT_FILENO);
		// printf("Finished\n");
	}
	return (0);
}
