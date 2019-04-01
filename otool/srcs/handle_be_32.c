#include "otool.h"
#include "libft.h"

void	handle_be_32_segment(struct load_command *lc, t_env *env)
{
	struct segment_command	*seg;
	struct section					*sec;
	uint32_t i;

	if (!(seg = (struct segment_command*)check_addr(lc,
		sizeof(struct segment_command), env)))
		return ;
	seg->nsects = reverse_int(seg->nsects);
	sec = (struct section*)((void*)lc + sizeof(struct segment_command));
	i = 0;
	while (i < seg->nsects && check_addr(sec, sizeof(struct section), env))
	{
		if (!ft_strcmp(sec->sectname, SECT_TEXT))
		{
			sec->offset = reverse_int(sec->offset);
			sec->size = reverse_int(sec->size);
			sec->addr = reverse_int(sec->addr);
			print_section_32(env, sec);
		}
		i++;
		sec++;
	}
}

void	handle_be_32(t_env *env)
{
	struct mach_header	*header;
	struct load_command		*lc;
	uint32_t				i;

	print_before(env);
	if (!(header = (struct mach_header*)check_addr(env->ptr,
		sizeof(struct mach_header), env)))
		return ;
	header->magic = reverse_int(header->magic);
	header->ncmds = reverse_int(header->ncmds);
	lc = env->ptr + sizeof(struct mach_header);
	i = 0;
	while (i < header->ncmds && check_addr(lc, sizeof(struct load_command), env))
	{
		lc->cmdsize = reverse_int(lc->cmdsize);
		lc->cmd = reverse_int(lc->cmd);
		if (lc->cmd == LC_SEGMENT)
			handle_be_32_segment(lc, env);
		lc = (void*)lc + lc->cmdsize;
		i++;
	}
}
