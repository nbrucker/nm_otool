#include "otool.h"
#include "libft.h"

void	handle_be_64_segment(struct load_command *lc, t_env *env)
{
	struct segment_command_64	*seg;
	struct section_64					*sec;
	uint32_t i;

	if (!(seg = (struct segment_command_64*)check_addr(lc,
		sizeof(struct segment_command_64), env)))
		return ;
	seg->nsects = reverse_int(seg->nsects);
	sec = (struct section_64*)((void*)lc + sizeof(struct segment_command_64));
	i = 0;
	while (i < seg->nsects && check_addr(sec, sizeof(struct section_64), env))
	{
		if (!ft_strcmp(sec->sectname, SECT_TEXT))
		{
			sec->offset = reverse_int(sec->offset);
			sec->size = reverse_long(sec->size);
			sec->addr = reverse_long(sec->addr);
			print_section_64(env, sec);
		}
		i++;
		sec++;
	}
}

void	handle_be_64(t_env *env)
{
	struct mach_header_64	*header;
	struct load_command		*lc;
	uint32_t				i;

	print_before(env);
	if (!(header = (struct mach_header_64*)check_addr(env->ptr,
		sizeof(struct mach_header_64), env)))
		return ;
	header->magic = reverse_int(header->magic);
	header->ncmds = reverse_int(header->ncmds);
	lc = env->ptr + sizeof(struct mach_header_64);
	i = 0;
	while (i < header->ncmds && check_addr(lc, sizeof(struct load_command), env))
	{
		lc->cmdsize = reverse_int(lc->cmdsize);
		lc->cmd = reverse_int(lc->cmd);
		if (lc->cmd == LC_SEGMENT_64)
			handle_be_64_segment(lc, env);
		lc = (void*)lc + lc->cmdsize;
		i++;
	}
}
