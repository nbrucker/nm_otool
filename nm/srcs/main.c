#include "libft.h"
#include "nm.h"

//pas ranger



 //ranger

void	nm(void *ptr, size_t size)
{
	uint32_t	magic;
	t_env			*env;

	if (!(env = init_env(ptr, size)))
		return ;
	if (size < 4)
		return ;
	magic = ((uint32_t*)ptr)[0];
	if (magic == MH_MAGIC_64)
		handle_64(env);
	else if (magic == MH_MAGIC)
		handle_32(env);
	else if (magic == MH_CIGAM)
		handle_be_32(env);
	else
		printf("%x\n", magic);
	free(env);
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
	ptr = mmap(NULL, buf.st_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
	if (ptr == MAP_FAILED)
		return (ft_putendl("mmap error"));
	if (ac > 2)
		print_file_name(str);
	nm(ptr, buf.st_size);
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
