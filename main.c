#include "minishell.h"

int lexer_input(t_token **token, char *input, t_env *env)
{
	int 	i;
	int 	len;
	int 	start;
	char	*part;
	bool	space;

	i = 0;
	space = false;
	// printf(BOLDGREEN "(LEXER_FUNCTION) \n" RESET);
	while (input[i])
	{
		if (input[i] && f_isspace(input[i]))															// Skip spaces
		{
			// printf(CYAN"find_space\n"RESET);
			space = true;																				// Flag for no space words
			i++;
		}
		else if (input[i] && is_quote(input[i]))														//	Inside quotes
		{
			// printf(YELLOW"find_quote\n"RESET);
			len = inside_quote(input, i, &part, env);
			if (len == -1 || !part)
				return (0);
			// printf(BLUE"word_inside_quote = %s\n", part);
			add_token(token, part, WORD, space);														// Create Node with its type
			space = false;
			i = len;
		}
		else if (input[i] && is_word_start(input[i]))													// If find word create a token for it 
		{
			// printf(MAGENTA"find_word | input[%c] | \n"RESET, input[i]);
			start = i;
			while (input[i] && is_word_start(input[i]))
				i++;
			part = ft_substr(input, start, i - start);
			if (!part)
				return (0);
			// printf(BLUE"word = %s\n"RESET, part);
			add_token(token, part, WORD, space);
			space = false;
		}
		else if (input[i] && is_operator(input[i]))														// Check for operator
			i = check_operator(input, i, token, space);
		else if (input[i] && input[i + 1] && valid_expand(input[i], input[i + 1]) == 1) 				// Expanding in normale
		{
			// printf(GREEN"expand_outside_quotes\n"RESET);
			if (input[i + 1] == '$')
				i += 1;
			i = expanding_var(token, i, input, env, space);
			if (i == -1)
				return (0);
			// handling '}' in last of ${USER} => [ouelkhar]
		}
		else
			i++;
	}
	if (i > 0)
		merge_words(token);																				// Merge words thath are no space bitween them 
	// printf("Last Tokens ===>>\n");
	// print_tokens(*token);
	return (1);
}

int parsing_function(t_token **token, char *input, char **env)
{
	t_env	*envr;
	t_cmd	*cmd;

	envr = collect_env(env); 																		// collecte environement variables
	if (!envr)
		return (0);																					//	return 0 for error & 1 for success 
	if (!check_quote(input))
	{
		write_error(2);
		free_env(envr);
		return (0);
	}
	if (!lexer_input(token, input, envr))
	{
		write_error(3);
		free_env(envr);
		return (0);
	}
	cmd = parse_cmd(token);
	if (!cmd)
		return (0);
	print_cmds(cmd);
	free_env(envr);
	return (1);
}

// void    ll(void)
// {
//     system("leaks -q minishell");
// }

int main(int argc, char **argv, char **env)
{
	t_token *token;
	char	*input;

	(void)argc;
	(void)argv;

	// atexit(ll);
	while (1)
	{
		input = readline(BOLDRED "[MINI_SHELL]$> " RESET);
		if (input)
		{
			token = NULL;
			add_history(input);
			printf(BOLDCYAN "INPUT BY READLINE [%s]\n" RESET, input);
			if (parsing_function(&token, input, env) == 0)
			{
				free_tokens(token); 											// function to free the linked list from tokens
				write_error(1);													// function to return error depends on num
			}
			free_tokens(token);
			free(input);
		}
		else
			return (1);
	}
	return (0);
}

