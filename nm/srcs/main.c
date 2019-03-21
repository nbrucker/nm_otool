#include "libft.h"
#include "nm.h"

//pas ranger



//ranger

int	nm(void *ptr, size_t size)
{
	uint32_t	magic;
	t_env			*env;

	if (!(env = init_env(ptr, size)))
		return (1);
	if (size < 4)
		return (1);
	magic = ((uint32_t*)ptr)[0];
	if (magic == MH_MAGIC_64)
		handle_64(env);
	else if (magic == MH_MAGIC)
		handle_32(env);
	else if (magic == MH_CIGAM)
		handle_be_32(env);
	else if (magic == MH_CIGAM_64)
		handle_be_64(env);
	else if (magic == FAT_MAGIC)
		handle_fat(env);
	else if (magic == FAT_CIGAM)
		handle_be_fat(env);
	else
		printf("%x\n", magic);
	print_cmds(env);
	free_cmds(env->cmd);
	free(env);
	return (env->error);
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
	ret = nm(ptr, buf.st_size);
	munmap(ptr, buf.st_size);
	close(fd);
	return (ret);
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
			if (handle_file(av[i], ac) == 1)
				return (1);
			i++;
		}
	}
	return (0);
}
