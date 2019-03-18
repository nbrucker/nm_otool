#include "libft.h"
#include "nm.h"

void	print_cmds(t_env *env, t_cmd *cmds)
{
	t_cmd	*next;

	sort_cmds(get_first_cmd(cmds));
	cmds = get_first_cmd(cmds);
	while (cmds)
	{
		next = cmds->next;
		if (env->type == 64)
			print_value_64(cmds->value, cmds->type);
		else
			print_value_32(cmds->value, cmds->type);
		ft_putchar(' ');
		ft_putchar(cmds->type);
		ft_putchar(' ');
		ft_putendl(cmds->name);
		free(cmds->name);
		free(cmds);
		cmds = next;
	}
}

void	print_file_name(char *str)
{
	ft_putchar('\n');
	ft_putstr(str);
	ft_putstr(":\n");
}

void	print_value_32(uint64_t value, char type)
{
	char	buf[8];
	int		i;
	int		tmp;

	if (type == 'U')
		return (ft_putstr("        "));
	ft_memset(buf, '0', 8);
	i = 0;
	while (value != 0 && i < 8)
	{
		tmp = value % 16;
		if (tmp < 10)
			buf[i] = 48 + tmp;
		else
			buf[i] = 97 + tmp - 10;
		value /= 16;
		i++;
	}
	i = 7;
	while (i >= 0)
	{
		ft_putchar(buf[i]);
		i--;
	}
}

void	print_value_64(uint64_t value, char type)
{
	char	buf[16];
	int		i;
	int		tmp;

	if (type == 'U')
		return (ft_putstr("                "));
	ft_memset(buf, '0', 16);
	i = 0;
	while (value != 0 && i < 16)
	{
		tmp = value % 16;
		if (tmp < 10)
			buf[i] = 48 + tmp;
		else
			buf[i] = 97 + tmp - 10;
		value /= 16;
		i++;
	}
	i = 15;
	while (i >= 0)
	{
		ft_putchar(buf[i]);
		i--;
	}
}
