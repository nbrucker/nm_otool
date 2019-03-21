#include "nm.h"

char	get_type_32(struct nlist list, t_env *env)
{
	char					r;
	unsigned char	c;

	c = list.n_type & N_TYPE;
	if (c == N_UNDF && list.n_value != 0)
		r = 'C';
	else if (c == N_UNDF)
		r = 'U';
	else if (c == N_ABS)
		r = 'A';
	else if (c == N_SECT)
		r = get_section_64(list.n_sect, env);
	else if (c == N_PBUD)
		r = 'U';
	else if (c == N_INDR)
		r = 'I';
	else
		r = '?';
	if (r != '?' && !(list.n_type & N_EXT))
		r += 32;
	return (r);
}

char	get_type_64(struct nlist_64 list, t_env *env)
{
	char					r;
	unsigned char	c;

	c = list.n_type & N_TYPE;
	if (c == N_UNDF && list.n_value)
		r = 'C';
	else if (c == N_UNDF)
		r = 'U';
	else if (c == N_ABS)
		r = 'A';
	else if (c == N_SECT)
		r = get_section_64(list.n_sect, env);
	else if (c == N_PBUD)
		r = 'U';
	else if (c == N_INDR)
		r = 'I';
	else
		r = '?';
	if (r != '?' && !(list.n_type & N_EXT))
		r += 32;
	return (r);
}

char	get_section_64(unsigned char s, t_env *env)
{
	if (s == env->t_index)
		return ('T');
	else if (s == env->d_index)
		return ('D');
	else if (s == env->b_index)
		return ('B');
	else
		return ('S');
}

t_cmd	*get_first_cmd(t_cmd *cmds)
{
	while (cmds && cmds->previous)
		cmds = cmds->previous;
	return (cmds);
}
