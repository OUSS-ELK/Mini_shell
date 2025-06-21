
#include "execution.h"

void executor(t_cmd *cmds, t_exec *exec)
{
	if (!cmds)
		return ;
	exec->env_arr = env_lst_to_arr(exec->env_lst); //todo utils
	// g_sig = 1;
	if (!exec_here_doc(cmds, exec))
		cmd_or_pipe(cmds, exec);
	free_cmd_list(cmds);
	free_str_arr(&exec->env_arr);
}
