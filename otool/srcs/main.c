#include "otool.h"
#include "libft.h"

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
	ft_putstr("Archive : ");
	ft_putendl(env->file);
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
		env->error = otool_inside(new, size - (new - (void*)name), env->file, name, 2);
		ptr += sizeof(struct ar_hdr) + size;
	}
}

int		is_arm_ppc(char *arch)
{
	if (ft_strncmp(arch, "arm", 3) == 0)
		return (1);
	if (ft_strncmp(arch, "ppc", 3) == 0)
		return (1);
	return (0);
}

void	print_before_lib(t_env *env)
{
	ft_putstr(env->file);
	ft_putchar('(');
	ft_putstr(env->lib_name);
	ft_putendl("):");
	ft_putendl("Contents of (__TEXT,__text) section");
}

void	print_before_fat(t_env *env)
{
	ft_putstr(env->file);
	ft_putstr(" (architecture ");
	ft_putstr(env->arch);
	ft_putendl("):");
	ft_putendl("Contents of (__TEXT,__text) section");
}

void	print_before_normal(t_env *env)
{
	ft_putstr(env->file);
	ft_putendl(":");
	ft_putendl("Contents of (__TEXT,__text) section");
}

void	print_before(t_env *env)
{
	if (env->type == 0)
		print_before_normal(env);
	else if (env->type == 1)
		print_before_fat(env);
	else if (env->type == 2)
		print_before_lib(env);
}

void	print_number(uint8_t value)
{
	if (value < 10)
		ft_putchar(value + 48);
	else
		ft_putchar(value + 97 - 10);
}

void	(*get_function(uint32_t magic32, uint64_t magic64))(t_env*)
{
	(void)magic64;
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

t_env	*init_env(void *ptr, size_t size, char *file)
{
	t_env			*env;

	if (!(env = (t_env*)malloc(sizeof(t_env))))
		return (NULL);
	ft_bzero((void*)env, sizeof(t_env));
  env->ptr = ptr;
  env->size = size;
	env->file = file;
	return (env);
}

int	otool(void *ptr, size_t size, char *file)
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
	ret = env->error;
	if (env->arch)
		free(env->arch);
	free(env);
	return (ret);
}

int	handle_file(char *str)
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
	ret = otool(ptr, buf.st_size, str);
	munmap(ptr, buf.st_size);
	close(fd);
	return (ret);
}

int		main(int ac, char **av)
{
	int	i;

	if (ac < 2)
		ft_putendl("./ft_otool [file...]");
	else
	{
		i = 1;
		while (i < ac)
		{
			if (handle_file(av[i]) == 1)
				return (1);
			i++;
		}
	}
	return (0);
}
