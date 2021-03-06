#include "nm.h"

void	handle_be_64(t_env *env)
{
	struct mach_header_64	*header;
	struct load_command		*lc;
	uint32_t				i;

	if (!(header = (struct mach_header_64*)check_addr(env->ptr,
		sizeof(struct mach_header_64), env)))
		return ;
	header->magic = reverse_int(header->magic);
	header->ncmds = reverse_int(header->ncmds);
	lc = env->ptr + sizeof(struct mach_header_64);
	i = 0;
	while (i < header->ncmds
		&& check_addr(lc, sizeof(struct load_command), env))
	{
		lc->cmdsize = reverse_int(lc->cmdsize);
		lc->cmd = reverse_int(lc->cmd);
		if (lc->cmd == LC_SEGMENT_64)
			handle_be_64_segment(lc, env);
		else if (lc->cmd == LC_SYMTAB)
			handle_be_64_symtab(lc, env);
		lc = (void*)lc + lc->cmdsize;
		i++;
	}
	if (env->error == 0)
		handle_64(env);
}

void	handle_be_64_segment(struct load_command *lc, t_env *env)
{
	struct segment_command_64	*seg;

	if (!(seg = (struct segment_command_64*)check_addr(lc,
		sizeof(struct segment_command_64), env)))
		return ;
	seg->nsects = reverse_int(seg->nsects);
}

void	handle_be_64_symtab(struct load_command *lc, t_env *env)
{
	struct symtab_command	*sym;
	uint32_t				i;
	struct nlist_64			*list;

	if (!(sym = (struct symtab_command*)check_addr(lc,
		sizeof(struct symtab_command), env)))
		return ;
	sym->stroff = reverse_int(sym->stroff);
	sym->symoff = reverse_int(sym->symoff);
	sym->nsyms = reverse_int(sym->nsyms);
	list = env->ptr + sym->symoff;
	i = 0;
	while (i < sym->nsyms && check_addr((void*)&(list[i]),
		sizeof(struct nlist_64), env))
	{
		list[i].n_un.n_strx = reverse_int(list[i].n_un.n_strx);
		list[i].n_value = reverse_long(list[i].n_value);
		i++;
	}
}
