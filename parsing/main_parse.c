#include "../minishell.h"

// int lexer_input(t_token **token, char *input, t_env *env)
// {
// 	int 	i;
// 	int 	len;
// 	int 	start;
// 	char	*part;
// 	bool	space;

// 	i = 0;
// 	space = false;
// 	// printf(BOLDGREEN "(LEXER_FUNCTION) \n" RESET);
// 	while (input[i])
// 	{
// 		if (input[i] && f_isspace(input[i]))															// Skip spaces
// 		{
// 			// printf(CYAN"find_space\n"RESET);
// 			space = true;																				// Flag for space after words
// 			i++;
// 		}
// 		else if (input[i] && is_quote(input[i]))														//	Inside quotes
// 		{
// 			// printf(YELLOW"find_quote\n"RESET);
// 			len = inside_quote(input, i, &part, env);
// 			if (len == -1 || !part)
// 				return (0);
// 			// printf(BLUE"word_inside_quote = %s\n", part);
// 			add_token(token, part, WORD, space);														// Create Node with its type
// 			space = f_isspace(input[i]);
// 			i = len;
// 		}
// 		else if (input[i] && is_word_start(input[i]))													// If find word create a token for it 
// 		{
// 			// printf(MAGENTA"find_word | input[%c] | \n"RESET, input[i]);
// 			start = i;
// 			while (input[i] && is_word_start(input[i]))
// 				i++;
// 			part = ft_substr(input, start, i - start);
// 			if (!part)
// 				return (0);
// 			// printf(BLUE"word = %s\n"RESET, part);
// 			add_token(token, part, WORD, space);
// 			space = f_isspace(input[i]);
// 		}
// 		else if (input[i] && is_operator(input[i]))														// Check for operator
// 			i = check_operator(input, i, token, space);
// 		else if (input[i] && input[i + 1] && valid_expand(input[i], input[i + 1]) == 1) 				// Expanding in normale
// 		{
// 			// printf(GREEN"expand_outside_quotes\n" RESET);// 			if (input[i + 1] == '$')
// 				i += 1;
// 			i = expanding_var(token, i, input, env, &space);
// 			if (i == -1)
// 				return (0);
// 		}
// 		else
// 			i++;
// 	}
// 	// printf("i = %d\n", i);
// 	// printf("Before merge ===>>\n");
// 	// print_tokens(*token);
// 	if (i > 0)
// 		merge_words(token);																				// Merge words thath are no space bitween them 
// 	// printf("Last Tokens ===>>\n");
// 	// print_tokens(*token);
// 	return (1);
// }

int	handle_expand(t_token **token, char *input, t_env *env, bool *space)
{
	if (is_operator(input[0]))
		return (check_operator(input, 0, token, *space));
	if (valid_expand(input[0], input[1]) == 1)
	{
		if (input[1] == '$')
			return (expanding_var(token, 1, input, env, space));
		return (expanding_var(token, 0, input, env, space));
	}
	return (1);
}

int	handle_word1(t_token **token, char *input, bool *space)
{
	int		i;
	char	*part;

	i = 0;
	while (input[i] && is_word_start(input[i]))
		i++;
	part = ft_substr(input, 0, i);
	if (!part)
		return (0);
	add_token(token, part, WORD, *space);
	*space = f_isspace(input[i]);
	return (i);
}

int	handle_quote(t_token **token, char *input, t_env *env, bool *space)
{
	char	*part;
	int		len;

	len = inside_quote(input, 0, &part, env);
	if (len == -1 || !part)
		return (0);
	add_token(token, part, WORD, *space);
	*space = f_isspace(input[len]);
	return (len);
}

int	lexer_input(t_token **token, char *input, t_env *env)
{
	int		i;
	bool	space;

	i = 0;
	space = false;
	while (input[i])
	{
		if (f_isspace(input[i]))
		{
			space = true;
			i++;
		}
		else if (is_quote(input[i]))
			i += handle_quote(token, input + i, env, &space);
		else if (is_word_start(input[i]))
			i += handle_word1(token, input + i, &space);
		else
			i += handle_expand(token, input + i, env, &space);
	}
	if (i > 0)
		merge_words(token);
	return (1);
}

int parsing_function(t_token **token, char *input, char **env, t_cmd **cmd)
{
	t_env	*envr;

	envr = collect_env(env); 																			// collecte environement variables
	if (!envr)
		return (0);																						//	return 0 for error & 1 for success 
	if (!check_quote(input))
	{
		write_error(2);
		free_env(envr);
		return (0);
	}
	if (!lexer_input(token, input, envr))
	{
		// printf("lexer function error\n");
		write_error(3);
		free_env(envr);
		return (0);
	}
	// printf("lexer function pass\n");
	*cmd = parse_cmd(token);
	if (!*cmd)
		return (0);
	// printf("parse cmd function pass\n");
	free_env(envr);
	return (1);
}
