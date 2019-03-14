#include "libft.h"
#include "nm.h"

void	print_cmds(t_cmd *cmds)
{
	sort_cmds(get_first_cmd(cmds));
	cmds = get_first_cmd(cmds);
	while (cmds)
	{
		print_value(cmds->value);
		ft_putchar(' ');
		ft_putchar(cmds->type);
		ft_putchar(' ');
		ft_putendl(cmds->name);
		cmds = cmds->next;
	}
}

void	print_file_name(char *str)
{
	ft_putchar('\n');
	ft_putstr(str);
	ft_putstr(":\n");
}

void	print_value(uint64_t value)
{
	char	buf[16];
	int		i;
	int		tmp;

	if (value == 0)
		return (ft_putstr("                "));
	ft_memset(buf, '0', 16);
	i = 0;
	while (value != 0)
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
