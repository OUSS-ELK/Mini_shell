#ifndef EXECUTION_H
# define EXECUTION_H

# include "../minishell.h"
# include <sys/wait.h>
# include <errno.h>

void executor(t_token *tokens, t_exec *exec);


// utils
void	free_2d(char **arr);

#endif