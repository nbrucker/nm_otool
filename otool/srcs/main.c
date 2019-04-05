#include "otool.h"
#include "libft.h"

int		is_arm_ppc(char *arch)
{
	if (ft_strncmp(arch, "arm", 3) == 0)
		return (1);
	if (ft_strncmp(arch, "ppc", 3) == 0)
		return (1);
	return (0);
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
