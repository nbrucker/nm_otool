#include "libft.h"
#include "nm.h"

void	sort_cmds(t_cmd *cmd)
{
	t_cmd *in;
	t_cmd *out;
	t_cmd *tmp;

	out = cmd;
	while (out)
	{
		in = out->next;
		while (in)
		{
			if (ft_strcmp(out->name, in->name) > 0)
			{
				swap_cmd(out, in);
				tmp = out;
				out = in;
				in = tmp;
			}
			in = in->next;
		}
		out = out->next;
	}
}

void	swap_all(t_cmd *a, t_cmd *b)
{
	t_cmd *next;
	t_cmd *previous;

	next = a->next;
	previous = a->previous;
	a->next = b->next;
	a->previous = b->previous;
	b->next = next;
	b->previous = previous;
	if (a->next)
		a->next->previous = a;
	if (a->previous)
		a->previous->next = a;
	if (b->next)
		b->next->previous = b;
	if (b->previous)
		b->previous->next = b;
}

void	swap_cmd(t_cmd *a, t_cmd *b)
{
	if (a->next == b)
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
	else
		swap_all(a, b);
}
