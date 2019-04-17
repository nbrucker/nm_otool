#include "otool.h"
#include "libft.h"

void	print_addr_32(uint64_t value)
{
	char	buf[8];
	int		i;
	int		tmp;

	ft_memset(buf, '0', 8);
	i = 0;
	while (value != 0 && i < 8)
	{
		tmp = value % 16;
		if (tmp < 10)
			buf[i] = 48 + tmp;
		else
			buf[i] = 97 + tmp - 10;
		value /= 16;
		i++;
	}
	i = 7;
	while (i >= 0)
	{
		ft_putchar(buf[i]);
		i--;
	}
}

void	reverse_section_32(t_env *env, struct section *sec)
{
	uint32_t *ptr;
	uint32_t i;

	if (!(ptr = (uint32_t*)check_addr(env->ptr + sec->offset, sec->size, env)))
		return ;
	i = 0;
	while (i * 4 < sec->size && check_addr(ptr + i, 4, env))
	{
		ptr[i] = reverse_int(ptr[i]);
		i++;
	}
}

void	print_section_32(t_env *env, struct section *sec)
{
	uint8_t		*ptr;
	uint32_t	i;
	uint32_t	j;

	if (is_arm_ppc(env->arch) && env->le == 1)
		reverse_section_32(env, sec);
	if (!(ptr = (uint8_t*)check_addr(env->ptr + sec->offset, sec->size, env)))
		return ;
	i = 0;
	while (i < sec->size && env->error == 0)
	{
		print_addr_32(sec->addr + i);
		ft_putchar('\t');
		j = 0;
		while (j < 16 && i + j < sec->size  && check_addr(ptr + i + j, 1, env))
		{
			print_number(ptr[i + j] / 16);
			print_number(ptr[i + j] % 16);
			if (!is_arm_ppc(env->arch) || (j + 1) % 4 == 0)
				ft_putchar(' ');
			j++;
		}
		ft_putchar('\n');
		i += 16;
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
			print_section_32(env, sec);
		i++;
		sec++;
	}
}

void	handle_32(t_env *env)
{
	struct mach_header		*h;
	struct load_command		*lc;
	uint32_t				i;

	env->le = 1;
	if (!(h = (struct mach_header*)check_addr(env->ptr,
		sizeof(struct mach_header), env)))
		return ;
	if (!(env->arch = ft_strdup(arch_get_name(h->cputype, h->cpusubtype))))
		return ;
	print_before(env);
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
