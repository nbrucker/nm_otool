#include "nm.h"
#include "libft.h"

int	error_opening_file(char *str)
{
	ft_putstr_fd("Can\'t open ", 2);
	ft_putendl_fd(str, 2);
	return (1);
}

int	error_not_file(char *str)
{
	ft_putstr_fd(str, 2);
	ft_putendl_fd(" is not a file", 2);
	return (1);
}

int ft_error(char *str)
{
	ft_putendl_fd(str, 2);
	return (1);
}

void	error_cmdsize(t_env *env)
{
	env->error = 1;
	ft_putendl_fd("wrong cmdsize", 2);
}
