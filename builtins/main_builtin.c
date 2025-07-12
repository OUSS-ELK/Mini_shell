#include "../minishell.h"

int	is_builtin(t_cmd *cmd)
{
	if (!cmd || !cmd->args || !cmd->args[0])
		return (0);
	if (ft_strcmp(cmd->args[0], "echo") == 0)
		return (1);
	else if (ft_strcmp(cmd->args[0], "cd") == 0)
		return (1);
	else if (ft_strcmp(cmd->args[0], "pwd") == 0)
		return (1);
	else if (ft_strcmp(cmd->args[0], "export") == 0)
		return (1);
	else if (ft_strcmp(cmd->args[0], "unset") == 0)
		return (1);
	else if (ft_strcmp(cmd->args[0], "env") == 0)
		return (1);
	else if (ft_strcmp(cmd->args[0], "exit") == 0)
		return (1);
	return (0);
}

int	run_builtin(t_cmd *cmd, t_env **env)
{
	int	result;

	if (!cmd || !cmd->args || !cmd->args[0])
		return (0);
	result = 0;
	if (ft_strcmp(cmd->args[0], "echo") == 0)
		result = ft_echo(cmd);
	else if (ft_strcmp(cmd->args[0], "cd") == 0)
		result = ft_cd(cmd->args, env);
	else if (ft_strcmp(cmd->args[0], "pwd") == 0)
		result = ft_pwd(env);
	// else if (ft_strcmp(cmd->args[0], "export") == 0)
	// 	result = ft_export(cmd->args, env);
	else if (ft_strcmp(cmd->args[0], "unset") == 0)
		result = ft_unset(cmd->args, env);
	else if (ft_strcmp(cmd->args[0], "env") == 0)
		result = ft_env(env);
	else if (ft_strcmp(cmd->args[0], "exit") == 0)
		result = ft_exit(cmd, env);
	g_exit_status = result;
	return (1);
}
