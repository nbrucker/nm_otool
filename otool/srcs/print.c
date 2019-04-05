#include "otool.h"
#include "libft.h"

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
