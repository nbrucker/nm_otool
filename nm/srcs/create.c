#include "libft.h"
#include "nm.h"

t_cmd	*create_cmd(t_cmd *cmds)
{
	t_cmd	*new;

	if (!(new = (t_cmd*)malloc(sizeof(t_cmd))))
		return (NULL);
	new->next = NULL;
	new->previous = NULL;
	if (cmds != NULL)
	{
		while (cmds && cmds->next)
			cmds = cmds->next;
		cmds->next = new;
		new->previous = cmds;
	}
	cmds = new;
	return (new);
}
