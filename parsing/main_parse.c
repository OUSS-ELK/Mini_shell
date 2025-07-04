#include "../minishell.h"

int handle_expansion(t_token **token, char *input, t_env *env, t_lexvars *st)
{
	int	new_i;

	new_i = expanding_var(token, st->i, input, env, &st->space, st->heredoc);
	if (new_i == -1)
		return (0);
	st->i = new_i;
	return (1);
}

int handle_operator(t_token **token, char *input, int i, t_lexvars *st)
{
	return check_operator(input, i, token, st->space, &st->heredoc);
}

int handle_word(t_token **token, char *input, t_lexvars *st)
{
	int		start;
	char	*part;

	start = st->i;
	while (input[st->i] && is_word_start(input[st->i]))
		st->i++;
	part = ft_substr(input, start, st->i - start);
	if (!part)
		return (0);
	add_token(token, part, WORD, st->space, false);
	st->space = f_isspace(input[st->i]);
	if (st->heredoc)
		st->heredoc = false;
	return (1);
}

int handle_quote(t_token **token, char *input, t_env *env, t_lexvars *st)
{
	char	*part;
	int		len;

	len = inside_quote(input, st->i, &part, env, &st->heredoc);
	if (len == -1 || !part)
		return (0);
	add_token(token, part, WORD, st->space, true);
	st->space = f_isspace(input[len]);
	st->i = len;
	if (st->heredoc)
		st->heredoc = false;
	return (1);
}

void handle_space(char *input, t_lexvars *st)
{
	while (input[st->i] && f_isspace(input[st->i]))
		st->i++;
	st->space = true;
}

void	else_case(t_token **token, char *input, t_lexvars *st)
{
	if (input[st->i + 1] && input[st->i] == '$' && input[st->i + 1] == '\0')
			add_token(token, "$", WORD, st->space, false);
	st->i++;
}

int lexing(t_token **token, char *input, t_env *env, t_lexvars *st)
{
	if (f_isspace(input[st->i]))
		handle_space(input, st);
	else if (is_quote(input[st->i]))
	{
		if (!handle_quote(token, input, env, st))
			return (0);
	}
	else if (is_word_start(input[st->i]))
	{
		if (!handle_word(token, input, st))
			return (0);
	}
	else if (is_operator(input[st->i]))
		st->i = handle_operator(token, input, st->i, st);
	else if (input[st->i + 1] && valid_expand(input[st->i], input[st->i + 1]))
	{
		if (!handle_expansion(token, input, env, st))
			return (0);
	}
	else
		else_case(token, input, st);
	return (1);
}


int lexer_input(t_token **token, char *input, t_env *env)
{
	t_lexvars	st;

	st.i = 0;
	st.space = false;
	st.heredoc = false;
	while (input[st.i])
	{
		if (!lexing(token, input, env, &st))
			return (0);
	}
	if (st.i > 0)
		merge_words(token);
	return (1);
}

int parsing_function(t_token **token, char *input, char **env, t_cmd **cmd)
{
	t_env	*envr;

	envr = collect_env(env); 																			// 	collecte environement variables
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

// int lexer_input(t_token **token, char *input, t_env *env)
// {
// 	int 	i;
// 	int 	len;
// 	int 	start;
// 	char	*part;
// 	bool	space;
// 	bool	heredoc;

// 	i = 0;
// 	space = false;
// 	heredoc = false;
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
// 			len = inside_quote(input, i, &part, env, &heredoc);
// 			if (len == -1 || !part)
// 				return (0);
// 			// printf(BLUE"word_inside_quote = %s\n", part);
// 			add_token(token, part, WORD, space, true);												// Create Node with its type
// 			space = f_isspace(input[i]);

// 			if (heredoc)
//     			heredoc = false;
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
// 			add_token(token, part, WORD, space, false);
// 			space = f_isspace(input[i]);
// 			if (heredoc)
//     			heredoc = false;
// 		}
// 		else if (input[i] && is_operator(input[i]))														// Check for operator
// 		{
// 			i = check_operator(input, i, token, space, &heredoc);
// 			// if (heredoc)
//     		// 	heredoc = false;
// 			// printf("after operator\n");
// 		}
// 		else if (input[i] && input[i + 1] && valid_expand(input[i], input[i + 1]) == 1) 				// Expanding in normale
// 		{
// 			printf(GREEN"expand_outside_quotes\n" RESET);
// 			// if (input[i + 1] == '$')
// 			// 	i += 1;
// 			i = expanding_var(token, i, input, env, &space, heredoc);
// 			if (i == -1)
// 				return (0);
// 		}
// 		else
// 		{
// 			printf("else\n");
// 			i++;
// 		}
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