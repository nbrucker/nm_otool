#include "libft.h"
#include "nm.h"

void	sort_cmds(t_cmd *cmds)
{
	int	count;

	count = 0;
	while (cmds && cmds->next)
	{
		if (ft_strcmp(cmds->name, cmds->next->name) > 0)
		{
			swap_cmd(cmds, cmds->next);
			count++;
		}
		else
			cmds = cmds->next;
		if (cmds->next == NULL && count > 0)
		{
			count = 0;
			cmds = get_first_cmd(cmds);
		}
	}
}

void	swap_cmd(t_cmd *a, t_cmd *b)
{
	a->next = b->next;
	b->previous = a->previous;
	a->previous = b;
	b->next = a;
	if (b->previous)
		b->previous->next = b;
	if (a->next)
		a->next->previous = a;
}
