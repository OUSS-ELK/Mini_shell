#include "minishell.h"

int f_strlen(char *s)
{
    int i;

    if (!s)
        return (0);
    i = 0;
    while (s[i])
        i++;
    return (i);
}

int f_isspace(char *s)
{
    while (*s)
    {
        if (*s == ' ' || (*s >= 9 && *s <= 13))
            return (0);
        s++;
    }
    return (1);
}

int lexer_function(t_token  **token, char *input)
{
    if (!token || !input)
        return (0);
    return (1);
}

void    ll(void)
{
    system("leaks -q minishell");
}

int main(void)
{
    char    *input;
    t_token *token;

    atexit(ll);
    while (1)
    {
        input = readline("[minishell]$ ");
        if (input)
        {
            if (strncmp(input, "exit", 4) == 0)
            {
                free(input);
                return (1);
            }
            if (f_strlen(input) > 0)
            {
                add_history(input);
                write_history("history");
                // printf("add_history\n");
            }
            printf("input reading by readline [%s]\n", input);
            if (!lexer_function(&token, input))
            {
                write_error(1);         // function to return error depends on num
                free_tokens(token);     // function to free the linked list from tokens
            }
            free(input);
        }
        else
            exit(1);
    }
}
