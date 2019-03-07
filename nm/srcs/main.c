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

void	print_cmds(t_cmd *cmds)
{
	sort_cmds(get_first_cmd(cmds));
	cmds = get_first_cmd(cmds);
	while (cmds)
	{
		printf("%016llx T %s\n", cmds->value, cmds->name);
		cmds = cmds->next;
	}
}

void	handle_64_symtab(struct load_command *lc, void* ptr)
{
	struct symtab_command *sym;
	uint32_t							i;
	char									*table;
	struct nlist_64				*list;
	t_cmd									*cmds;

	cmds = NULL;
	sym = (struct symtab_command*)lc;
	table = ptr + sym->stroff;
	list = ptr + sym->symoff;
	i = 0;
	while (i < sym->nsyms)
	{
		//printf("%s: %llx\n", table + list[i].n_un.n_strx, list[i].n_value);
		if (!(cmds = create_cmd(cmds)))
			return(ft_putendl("malloc error"));
		cmds->name = table + list[i].n_un.n_strx;
		cmds->value = list[i].n_value;
		i++;
	}
	print_cmds(cmds);
}

void	handle_64(void *ptr)
{
	struct mach_header_64	*header;
	struct load_command		*lc;
	uint32_t							i;

	header = (struct mach_header_64*)ptr;
	lc = ptr + sizeof(struct mach_header_64);
	i = 0;
	while (i < header->ncmds)
	{
		if (lc->cmd == LC_SYMTAB)
			handle_64_symtab(lc, ptr);
		lc = (void*)lc + lc->cmdsize;
		i++;
	}
}

void	nm(void *ptr)
{
	unsigned int	magic;

	magic = ((unsigned int*)ptr)[0];
	if (magic == MH_MAGIC_64)
		handle_64(ptr);
}

void	handle_file(char *str)
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
	nm(ptr);
	munmap(ptr, buf.st_size);
}

int		main(int ac, char **av)
{
	int	i;

	if (ac < 2)
		handle_file("a.out");
	else
	{
		i = 1;
		while (i < ac)
		{
			handle_file(av[i]);
			i++;
		}
	}
	return (0);
}
