#include "libft.h"
#include "nm.h"

void	set_ind_cmd_name(t_cmd *cmd)
{
	char *tmp;

	cmd = get_first_cmd(cmd);
	while (cmd)
	{
		if (cmd->i_name)
		{
			//protege malloc
			tmp = cmd->name;
			cmd->name = ft_strjoin(cmd->name, " (indirect for ");
			free(tmp);
			tmp = cmd->name;
			cmd->name = ft_strjoin(cmd->name, cmd->i_name);
			free(tmp);
			tmp = cmd->name;
			cmd->name = ft_strjoin(cmd->name, ")");
			free(tmp);
		}
		cmd = cmd->next;
	}
}

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
			if (ft_strcmp(out->name, in->name) > 0
				|| (ft_strcmp(out->name, in->name) == 0 && out->value > in->value))
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
