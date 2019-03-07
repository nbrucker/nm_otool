#include "nm.h"
#include "libft.h"

void	error_opening_file(char *str)
{
	ft_putstr("Can\'t open ");
	ft_putstr(str);
	ft_putchar('\n');
}

void	error_not_file(char *str)
{
	ft_putstr(str);
	ft_putendl(" is not a file");
}