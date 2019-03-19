#include "libft.h"
#include "nm.h"

t_cmd	*error_cmd(t_env *env, t_cmd *cmd, t_cmd *new)
{
	t_cmd	*next;

	env->error = 1;
	cmd = get_first_cmd(cmd);
	while (cmd)
	{
		next = cmd->next;
		free(cmd->name);
		free(cmd);
		cmd = next;
	}
	if (new && new->name)
		free(new->name);
	if (new)
		free(new);
	return (NULL);
}

t_cmd	*create_cmd(t_cmd *cmds, char *table, struct nlist_64 list, t_env *env)
{
	t_cmd	*new;

	if (!(new = (t_cmd*)malloc(sizeof(t_cmd))))
		return (error_cmd(env, cmd, new));
	new->next = NULL;
	new->previous = NULL;
	if (cmds != NULL)
	{
		while (cmds && cmds->next)
			cmds = cmds->next;
		cmds->next = new;
		new->previous = cmds;
	}
	if (check_addr(table + list.n_un.n_strx, 1, env))
		new->name = ft_strdup(table + list.n_un.n_strx);
	else
		new->name = ft_strdup("bad string index");
	if (!new->name)
		return (error_cmd(env, cmd, new));
	new->value = list.n_value;
	new->type = get_type_64(list, env);
	cmds = new;
	return (new);
}

t_cmd	*create_cmd_32(t_cmd *cmds, char *table, struct nlist list, t_env *env)
{
	t_cmd	*new;

	if (!(new = (t_cmd*)malloc(sizeof(t_cmd))))
		return (error_cmd(env, cmd, new));
	new->next = NULL;
	new->previous = NULL;
	if (cmds != NULL)
	{
		while (cmds && cmds->next)
			cmds = cmds->next;
		cmds->next = new;
		new->previous = cmds;
	}
	if (check_addr(table + list.n_un.n_strx, 1, env))
		new->name = ft_strdup(table + list.n_un.n_strx);
	else
		new->name = ft_strdup("bad string index");
	if (!new->name)
		return (error_cmd(env, cmd, new));
	new->value = list.n_value;
	new->type = get_type_32(list, env);
	cmds = new;
	return (new);
}
