#include "nm.h"
#include "libft.h"

void	print_fat_arch(t_env *env, char *name)
{
	ft_putchar('\n');
	ft_putstr(env->file);
	ft_putstr(" (for architecture ");
	ft_putstr(name);
	ft_putendl("):");
}

void	print_ar_name(t_env *env, char *name)
{
	ft_putchar('\n');
	ft_putstr(env->file);
	ft_putchar('(');
	ft_putstr(name);
	ft_putendl("):");
}
