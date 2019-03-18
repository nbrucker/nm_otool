#include "nm.h"
#include "libft.h"

int	error_opening_file(char *str)
{
	ft_putstr("Can\'t open ");
	ft_putstr(str);
	ft_putchar('\n');
	return (-1);
}

int	error_not_file(char *str)
{
	ft_putstr(str);
	ft_putendl(" is not a file");
	return (-1);
}

int ft_error(char *str)
{
	ft_putendl(str);
	return (-1);
}

void	error_cmdsize(t_env *env)
{
	env->error = 1;
	ft_putendl("wrong cmdsize");
}
