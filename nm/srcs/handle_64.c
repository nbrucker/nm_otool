#include "libft.h"
#include "nm.h"

void	handle_64(t_env *env)
{
	struct mach_header_64	*header;
	struct load_command		*lc;

	lc = NULL;
	if (!(header = (struct mach_header_64*)check_addr(env->ptr,
		sizeof(struct mach_header_64), env)))
		return ;
	h64_seg(header, lc, env);
	h64_sym(header, lc, env);
}

void	h64_seg(struct mach_header_64 *h, struct load_command *lc, t_env *env)
{
	uint32_t i;

	lc = env->ptr + sizeof(struct mach_header_64);
	i = 0;
	while (i < h->ncmds && check_addr(lc, sizeof(struct load_command), env)
		&& env->error == 0)
	{
		if (lc->cmdsize < 1 || lc->cmdsize % 8 != 0)
			return (error_cmdsize(env));
		if (lc->cmd == LC_SEGMENT_64)
			handle_64_segment(lc, env);
		lc = (void*)lc + lc->cmdsize;
		i++;
	}
}

void	h64_sym(struct mach_header_64 *h, struct load_command *lc, t_env *env)
{
	uint32_t i;

	lc = env->ptr + sizeof(struct mach_header_64);
	i = 0;
	while (i < h->ncmds && check_addr(lc, sizeof(struct load_command), env)
		&& env->error == 0)
	{
		if (lc->cmd == LC_SYMTAB)
			return (handle_64_symtab(lc, env));
		lc = (void*)lc + lc->cmdsize;
		i++;
	}
	if (i == h->ncmds)
		ft_putendl_fd("warning: ft_nm: no name list", 2);
}

void	handle_64_segment(struct load_command *lc, t_env *env)
{
	struct segment_command_64	*seg;
	struct section_64					*sec;
	uint32_t									i;

	if (!(seg = (struct segment_command_64*)check_addr(lc,
		sizeof(struct segment_command_64), env)))
		return ;
	sec = (struct section_64*)((void*)lc + sizeof(struct segment_command_64));
	i = 0;
	while (i < seg->nsects && check_addr(sec, sizeof(struct section_64), env))
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

void	handle_64_symtab(struct load_command *lc, t_env *env)
{
	struct symtab_command	*sym;
	uint32_t				i;
	char					*table;
	struct nlist_64			*list;
	t_cmd					*cmds;

	cmds = NULL;
	if (!(sym = (struct symtab_command*)check_addr(lc,
		sizeof(struct symtab_command), env)))
		return ;
	table = env->ptr + sym->stroff;
	list = env->ptr + sym->symoff;
	i = 0;
	while (i < sym->nsyms && check_addr((void*)&(list[i]),
		sizeof(struct nlist_64), env))
	{
		if (!(list[i].n_type & N_STAB))
			if (!(cmds = create_cmd(cmds, table, list[i], env)))
				return ;
		i++;
	}
	if (env->error == 1)
		return (free_cmds(cmds));
	env->type = 64;
	env->cmd = cmds;
}
