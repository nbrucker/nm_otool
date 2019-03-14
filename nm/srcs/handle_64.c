#include "libft.h"
#include "nm.h"

void	handle_64(t_env *env)
{
	struct mach_header_64	*header;
	struct load_command		*lc;
	uint32_t				i;

	header = (struct mach_header_64*)env->ptr;
	lc = env->ptr + sizeof(struct mach_header_64);
	i = 0;
	while (i < header->ncmds)
	{
		if (lc->cmd == LC_SEGMENT_64)
			handle_64_segment(lc, env);
		lc = (void*)lc + lc->cmdsize;
		i++;
	}
	lc = env->ptr + sizeof(struct mach_header_64);
	i = 0;
	while (i < header->ncmds)
	{
		if (lc->cmd == LC_SYMTAB)
			handle_64_symtab(lc, env);
		lc = (void*)lc + lc->cmdsize;
		i++;
	}
}

void	handle_64_segment(struct load_command *lc, t_env *env)
{
	struct segment_command_64	*seg;
	struct section_64					*sec;
	uint32_t									i;

	seg = (struct segment_command_64*)lc;
	sec = (struct section_64*)((void*)lc + sizeof(struct segment_command_64));
	i = 0;
	while (i < seg->nsects)
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
	sym = (struct symtab_command*)lc;
	table = env->ptr + sym->stroff;
	list = env->ptr + sym->symoff;
	i = 0;
	while (i < sym->nsyms)
	{
		if (!(list[i].n_type & N_STAB))
		{
			if (!(cmds = create_cmd(cmds)))
				return (ft_putendl("malloc error"));
			cmds->name = table + list[i].n_un.n_strx;
			cmds->value = list[i].n_value;
			cmds->type = get_type_64(list[i], env);
		}
		i++;
	}
	print_cmds(cmds);
}
