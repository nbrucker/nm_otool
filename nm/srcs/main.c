#include "nm.h"
#include "libft.h"

void	nm(void *ptr)
{
	uint	magic;

	magic = ((uint*)ptr)[0];
	if (magic == MH_MAGIC_64)
		ft_putendl("aaaaa");
	printf("%x\n", magic);
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
