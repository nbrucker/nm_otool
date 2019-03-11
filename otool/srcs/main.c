#include "otool.h"
#include "libft.h"

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
			ft_putendl(av[i]);
			i++;
		}
	}
	return (0);
}
