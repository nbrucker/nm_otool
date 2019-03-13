#include "nm.h"
#include "libft.h"

t_cmd	*create_cmd(t_cmd *cmds)
{
	t_cmd	*new;

	if (!(new = (t_cmd*)malloc(sizeof(t_cmd))))
		return (NULL);
	new->next = NULL;
	new->previous = NULL;
	if (cmds != NULL)
	{
		while (cmds && cmds->next)
			cmds = cmds->next;
		cmds->next = new;
		new->previous = cmds;
	}
	cmds = new;
	return (new);
}

t_cmd	*get_first_cmd(t_cmd *cmds)
{
	while (cmds && cmds->previous)
		cmds = cmds->previous;
	return (cmds);
}

void	swap_cmd(t_cmd *a, t_cmd *b)
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

void	sort_cmds(t_cmd *cmds)
{
	int	count;

	count = 0;
	while (cmds && cmds->next)
	{
		if (ft_strcmp(cmds->name, cmds->next->name) > 0)
		{
			swap_cmd(cmds, cmds->next);
			count++;
		}
		else
			cmds = cmds->next;
		if (cmds->next == NULL && count > 0)
		{
			count = 0;
			cmds = get_first_cmd(cmds);
		}
	}
}

void	print_value(uint64_t value)
{
	char	buf[16];
	int		i;
	int		tmp;

	if (value == 0)
		return (ft_putstr("                "));
	ft_memset(buf, '0', 16);
	i = 0;
	while (value != 0)
	{
		tmp = value % 16;
		if (tmp < 10)
			buf[i] = 48 + tmp;
		else
			buf[i] = 97 + tmp - 10;
		value /= 16;
		i++;
	}
	i = 15;
	while (i >= 0)
	{
		ft_putchar(buf[i]);
		i--;
	}
}

void	print_cmds(t_cmd *cmds)
{
	sort_cmds(get_first_cmd(cmds));
	cmds = get_first_cmd(cmds);
	while (cmds)
	{
		print_value(cmds->value);
		ft_putchar(' ');
		ft_putchar(cmds->type);
		ft_putchar(' ');
		ft_putendl(cmds->name);
		cmds = cmds->next;
	}
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

char	get_type_64(struct nlist_64 list, t_env *env)
{
	char					r;
	unsigned char	c;

	c = list.n_type & N_TYPE;
	if (c == N_UNDF && list.n_type & N_EXT && list.n_value)
		r = 'C';
	else if (c == N_UNDF)
		r = 'U';
	else if (c == N_ABS)
		r = 'A';
	else if (c == N_SECT)
		r = get_section_64(list.n_sect, env);
	else if (c == N_PBUD)
		r = 'P';
	else if (c == N_INDR)
		r = 'I';
	else
		r = '?';
	if (r != '?' && !(list.n_type & N_EXT))
		r += 32;
	return (r);
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
		if (!(cmds = create_cmd(cmds)))
			return (ft_putendl("malloc error"));
		cmds->name = table + list[i].n_un.n_strx;
		cmds->value = list[i].n_value;
		cmds->type = get_type_64(list[i], env);
		i++;
	}
	print_cmds(cmds);
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

t_env	*init_env(void)
{
	t_env			*env;

	if (!(env = (t_env*)malloc(sizeof(t_env))))
		return (NULL);
	ft_bzero((void*)env, sizeof(t_env));
	env->sec_index = 1;
	return (env);
}

void	nm(void *ptr)
{
	uint32_t	magic;
	t_env			*env;

	if (!(env = init_env()))
		return ;
	env->ptr = ptr;
	magic = ((uint32_t*)ptr)[0];
	if (magic == MH_MAGIC_64)
		handle_64(env);
	else
		printf("%x\n", magic);
	free(env);
}

void	print_file_name(char *str)
{
	ft_putchar('\n');
	ft_putstr(str);
	ft_putstr(":\n");
}

void	handle_file(char *str, int ac)
{
	int			fd;
	struct stat	buf;
	void		*ptr;

	if ((fd = open(str, O_RDONLY)) < 0)
		return (error_opening_file(str));
	if (fstat(fd, &buf) < 0)
		return (ft_putendl("fstat error"));
	if (!S_ISREG(buf.st_mode))
		return (error_not_file(str));
	ptr = mmap(NULL, buf.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
	if (ptr == MAP_FAILED)
		return (ft_putendl("mmap error"));
	if (ac > 2)
		print_file_name(str);
	nm(ptr);
	munmap(ptr, buf.st_size);
	close(fd);
}

int		main(int ac, char **av)
{
	int	i;

	if (ac < 2)
		handle_file("a.out", ac);
	else
	{
		i = 1;
		while (i < ac)
		{
			handle_file(av[i], ac);
			i++;
		}
	}
	return (0);
}
