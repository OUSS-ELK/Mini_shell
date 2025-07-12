#include "../minishell.h"

// void	ft_free_isdir(char **cmd_path, char **cmd_name, t_cmdarg *current_cmd)
// {
// 	free(*cmd_path);
// 	free(*cmd_name);
// 	ft_cmd_error(current_cmd->cmd[0], "is a directory\n", 126);
// }
void	cleanup_dir_error(char **exec_path, char **command, t_cmd *cmd)
{
    free(*exec_path);
    free(*command);
    custom_error(cmd->args[0], "is a directory\n", 126);
}

// void	handle_exec_error(char *cmd_path, char *cmd_name)
// {
// 	free(cmd_path);
// 	free(cmd_name);
// 	ft_cmd_error(NULL, "malloc failure\n", 1);
// }

void	cleanup_malloc_error(char *exec_path, char *command)
{
    free(exec_path);
    free(command);
    custom_error(NULL, "Envp failure\n", 1);
}



// void	ft_free_and_error(char **cmd_path, char **cmd_name, char **envp)
// {
// 	free(*cmd_path);
// 	*cmd_path = NULL;
// 	free_dp(envp);
// 	*envp = NULL;
// 	free(*cmd_name);
// 	*cmd_name = NULL;
// 	if (errno == EACCES)
// 		ft_cmd_error(NULL, "Permission denied\n", 126);
// 	else
// 		ft_cmd_error(NULL, "execution failure\n", 1);
// }

void	cleanup_exec_error(char **exec_path, char **command, char **env_array)
{
    free(*exec_path);
    *exec_path = NULL;
    ft_free_array(env_array);
    *env_array = NULL;
    free(*command);
    *command = NULL;
    if (errno == EACCES)
        custom_error(NULL, "Permission denied\n", 126);
    else
        custom_error(NULL, "execution failure\n", 1);
}

void	ft_free_array(char **array)
{
    int i;

    if (!array)
        return;
    i = 0;
    while (array[i])
    {
        free(array[i]);
        array[i] = NULL;
        i++;
    }
    free(array);
}


/* Print and exit for “cmd not found” / “no such file” */
void	ft_print_exit(t_cmd *cmd, int no_file)
{
	if (no_file)
		custom_error(cmd->args[0], "No such file or directory\n", 127);
	else
		custom_error(cmd->args[0], "command not found\n", 127);
}