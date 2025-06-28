#ifndef BUILTINS_H
# define BUILTINS_H

# include "../minishell.h"
# include "../exec/execution.h"

// cd.c
void	builtin_cd(char **args, t_env **env_lst, t_exec *exec);

// export.c
void	builtin_export(char **av, t_env **env_lst, t_exec *exec);
void	print_export(t_env **env_lst);

// export_utils.c
char	*get_key(char *str);
char	*get_value(char *str);
void	add_to_env(t_env **env_lst, char *key, char *value);
void	append_env_value(t_env *tmp, char *value);
void	replace_env_value(t_env *tmp, char *value);


#endif