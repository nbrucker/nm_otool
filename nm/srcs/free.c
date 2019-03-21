#include "nm.h"

void	free_cmds(t_cmd *cmd)
{
	t_cmd	*next;

	cmd = get_first_cmd(cmd);
	while (cmd)
	{
		next = cmd->next;
		free(cmd->name);
		free(cmd);
		cmd = next;
	}
}
