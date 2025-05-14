#include "minishell.h"

int main(void)
{
    char *input;

    while (1)
    {
    
        input = readline("minishel>$");
        if (input)
        {
            printf("%s\n", input);
            break;
        }
    }
}