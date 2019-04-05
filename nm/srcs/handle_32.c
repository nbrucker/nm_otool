#include "nm.h"
#include "libft.h"

void	handle_32(t_env *env)
{
	struct mach_header		*header;
	struct load_command		*lc;

	lc = NULL;
	if (!(header = (struct mach_header*)check_addr(env->ptr,
		sizeof(struct mach_header), env)))
		return ;
	h32_seg(header, lc, env);
	h32_sym(header, lc, env);
}

void	h32_seg(struct mach_header *h, struct load_command *lc, t_env *env)
{
	uint32_t i;

	lc = env->ptr + sizeof(struct mach_header);
	i = 0;
	while (i < h->ncmds && check_addr(lc, sizeof(struct load_command), env)
		&& env->error == 0)
	{
		if (lc->cmdsize < 1 || lc->cmdsize % 4 != 0)
			return (error_cmdsize(env));
		if (lc->cmd == LC_SEGMENT)
			handle_32_segment(lc, env);
		lc = (void*)lc + lc->cmdsize;
		i++;
	}
}

void	h32_sym(struct mach_header *h, struct load_command *lc, t_env *env)
{
	uint32_t i;

	lc = env->ptr + sizeof(struct mach_header);
	i = 0;
	while (i < h->ncmds && check_addr(lc, sizeof(struct load_command), env)
		&& env->error == 0)
	{
		if (lc->cmd == LC_SYMTAB)
			return (handle_32_symtab(lc, env));
		lc = (void*)lc + lc->cmdsize;
		i++;
	}
}

void	handle_32_segment(struct load_command *lc, t_env *env)
{
	struct segment_command	*seg;
	struct section			*sec;
	uint32_t				i;

	if (!(seg = (struct segment_command*)check_addr(lc,
		sizeof(struct segment_command), env)))
		return ;
	sec = (struct section*)((void*)lc + sizeof(struct segment_command));
	i = 0;
	while (i < seg->nsects && check_addr(sec, sizeof(struct section), env))
	{
		if (!ft_strcmp(sec->sectname, SECT_TEXT))
			env->t_index = env->sec_index;
		else if (!ft_strcmp(sec->sectname, SECT_DATA))
			env->d_index = env->sec_index;
		else if (!ft_strcmp(sec->sectname, SECT_BSS))
			env->b_index = env->sec_index;
		env->sec_index++;
		i++;
		sec++;
	}
}

void	handle_32_symtab(struct load_command *lc, t_env *env)
{
	struct symtab_command	*sym;
	uint32_t				i;
	char					*table;
	struct nlist			*list;
	t_cmd					*cmds;

	cmds = NULL;
	if (!(sym = (struct symtab_command*)check_addr(lc,
		sizeof(struct symtab_command), env)))
		return ;
	table = env->ptr + sym->stroff;
	list = env->ptr + sym->symoff;
	i = 0;
	while (i < sym->nsyms && check_addr((void*)&(list[i]),
		sizeof(struct nlist), env))
	{
		if (!(list[i].n_type & N_STAB))
			if (!(cmds = create_cmd_32(cmds, table, list[i], env)))
				return ;
		i++;
	}
	if (env->error == 1)
		return (free_cmds(cmds));
	env->type = 32;
	env->cmd = cmds;
}
