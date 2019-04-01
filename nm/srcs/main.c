#include "libft.h"
#include "nm.h"

//pas ranger

void	set_error(t_env *env)
{
	env->error = 1;
}

int	get_ar_size(struct ar_hdr *hdr)
{
	char tmp[11];

	ft_bzero(tmp, 11);
	ft_memcpy(tmp, hdr->ar_size, 10);
	return (ft_atoi(tmp));
}

void	*get_ar_addr(char *name)
{
	name += ft_strlen(name);
	while (!name[0])
		name++;
	return ((void*)name);
}

void	print_fat_arch(t_env *env, char *name)
{
	ft_putchar('\n');
	ft_putstr(env->file);
	ft_putstr(" (for architecture ");
	ft_putstr(name);
	ft_putendl("):");
}

void	print_ar_name(t_env *env, char *name)
{
	ft_putchar('\n');
	ft_putstr(env->file);
	ft_putchar('(');
	ft_putstr(name);
	ft_putendl("):");
}

int	check_ar_name(void *addr, t_env *env)
{
	char *name;

	name = (char*)addr;
	while (check_addr((void*)name, 1, env) && name[0])
		name++;
	while (check_addr((void*)name, 1, env) && !name[0])
		name++;
	return (env->error);
}

int	nm_inside(void *ptr, size_t size, char *file, char *name, int type)
{
	void (*f)(t_env*);
	t_env			*env;
	int				ret;

	if (!(env = init_env(ptr, size, file)))
		return (1);
	if (size < 8)
		return (1);
	f = get_function(((uint32_t*)ptr)[0], ((uint64_t*)ptr)[0]);
	if (f)
		f(env);
	else
		error_file_format(env);
	if (type == 1)
		print_ar_name(env, name);
	else if (type == 2)
		print_fat_arch(env, name);
	print_cmds(env);
	free_cmds(env->cmd);
	ret = env->error;
	free(env);
	return (ret);
}

void	handle_lib(t_env *env)
{
	struct ar_hdr *hdr;
	void *ptr;
	int size;
	char *name;
	void *new;

	ptr = env->ptr + 8;
	if (!check_addr(ptr, sizeof(struct ar_hdr), env))
		return ;
	hdr = (struct ar_hdr*)ptr;
	size = get_ar_size(hdr);
	ptr += sizeof(struct ar_hdr) + size;
	while (ptr < env->ptr + env->size && env->error == 0 && check_addr(ptr, sizeof(struct ar_hdr), env))
	{
		hdr = (struct ar_hdr*)ptr;
		size = get_ar_size(hdr);
		if (check_ar_name(ptr + sizeof(struct ar_hdr), env))
			return ;
		name = ptr + sizeof(struct ar_hdr);
		new = get_ar_addr(name);
		env->error = nm_inside(new, size - (new - (void*)name), env->file, name, 1);
		ptr += sizeof(struct ar_hdr) + size;
	}
}

void	(*get_function(uint32_t magic32, uint64_t magic64))(t_env*)
{
	if (magic32 == MH_MAGIC_64)
		return (&handle_64);
	else if (magic32 == MH_CIGAM_64)
		return (&handle_be_64);
	else if (magic32 == MH_MAGIC)
		return (&handle_32);
	else if (magic32 == MH_CIGAM)
		return (&handle_be_32);
	else if (magic32 == FAT_MAGIC)
		return (&handle_fat);
	else if (magic32 == FAT_CIGAM)
		return (&handle_be_fat);
	else if (magic64 == LIB_MAGIC || magic64 == LIB_CIGAM)
		return (&handle_lib);
	return (NULL);
}

//ranger

int	nm(void *ptr, size_t size, char *file)
{
	void (*f)(t_env*);
	t_env			*env;
	int				ret;

	if (!(env = init_env(ptr, size, file)))
		return (1);
	if (size < 8)
		return (1);
	f = get_function(((uint32_t*)ptr)[0], ((uint64_t*)ptr)[0]);
	if (f)
		f(env);
	else
		error_file_format(env);
	if (env->cmd)
		print_cmds(env);
	free_cmds(env->cmd);
	ret = env->error;
	free(env);
	return (ret);
}

int	handle_file(char *str, int ac)
{
	int			fd;
	struct stat	buf;
	void		*ptr;
	int			ret;

	if ((fd = open(str, O_RDONLY)) < 0)
		return (error_opening_file(str));
	if (fstat(fd, &buf) < 0)
		return (ft_error("fstat error"));
	if (!S_ISREG(buf.st_mode))
		return (error_not_file(str));
	ptr = mmap(NULL, buf.st_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
	if (ptr == MAP_FAILED)
		return (ft_error("mmap error"));
	if (ac > 2)
		print_file_name(str);
	ret = nm(ptr, buf.st_size, str);
	munmap(ptr, buf.st_size);
	close(fd);
	return (ret);
}

int		main(int ac, char **av)
{
	int	i;

	if (ac < 2)
		return (handle_file("a.out", ac));
	else
	{
		i = 1;
		while (i < ac)
		{
			if (handle_file(av[i], ac) == 1)
				return (1);
			i++;
		}
	}
	return (0);
}
