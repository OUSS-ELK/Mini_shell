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
            return 0;
        s++;
    }
    return (1);
}

void    ll(void)
{
    system("leaks -q minishell");
}

int main(void)
{
    char *input;

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
            printf("%s\n", input);
            free(input);
        }
        else if (f_strlen(input) > 0)
            add_history(input);
        else
            exit(1);
    }
}
