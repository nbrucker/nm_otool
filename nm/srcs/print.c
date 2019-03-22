#include "libft.h"
#include "nm.h"

void	print_cmds(t_env *env)
{
	t_cmd	*next;
	t_cmd *cmd;

	if (!env->cmd)
		return ;
	cmd = env->cmd;
	sort_cmds(get_first_cmd(cmd));
	cmd = get_first_cmd(cmd);
	while (cmd)
	{
		next = cmd->next;
		if (env->type == 64)
			print_value_64(cmd->value, cmd->type);
		else
			print_value_32(cmd->value, cmd->type);
		ft_putchar(' ');
		ft_putchar(cmd->type);
		ft_putchar(' ');
		ft_putendl(cmd->name);
		cmd = next;
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

	if (type == 'U' || type == 'u')
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

	if (type == 'U' || type == 'u')
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
