#include "libft.h"
#include "nm.h"

t_cmd	*error_cmd(t_env *env, t_cmd *cmd, t_cmd *new)
{
	env->error = 1;
	free_cmds(cmd);
	if (new && new->name)
		free(new->name);
	if (new && new->i_name)
		free(new->i_name);
	if (new)
		free(new);
	return (NULL);
}

char	*cmd_get_name(t_env *env, char *addr)
{
	if (check_addr(addr, 1, env))
		return (ft_strdup(addr));
	else
	{
		env->error = 0;
		return (ft_strdup("bad string index"));
	}
}

t_cmd	*create_cmd(t_cmd *cmds, char *table, struct nlist_64 list, t_env *env)
{
	t_cmd	*new;

	if (!(new = (t_cmd*)malloc(sizeof(t_cmd))))
		return (error_cmd(env, cmds, new));
	ft_bzero((void*)new, sizeof(t_cmd));
	if ((cmds = get_last_cmd(cmds)))
	{
		cmds->next = new;
		new->previous = cmds;
	}
	new->value = list.n_value;
	new->type = get_type_64(list, env);
	if (new->type == 'I' || new->type == 'i')
		if (!(new->i_name = cmd_get_name(env, table + list.n_value)))
			return (error_cmd(env, cmds, new));
	new->name = cmd_get_name(env, table + list.n_un.n_strx);
	if (!new->name)
		return (error_cmd(env, cmds, new));
	cmds = new;
	return (new);
}

t_cmd	*create_cmd_32(t_cmd *cmds, char *table, struct nlist list, t_env *env)
{
	t_cmd	*new;

	if (!(new = (t_cmd*)malloc(sizeof(t_cmd))))
		return (error_cmd(env, cmds, new));
	ft_bzero((void*)new, sizeof(t_cmd));
	if ((cmds = get_last_cmd(cmds)))
	{
		cmds->next = new;
		new->previous = cmds;
	}
	new->value = list.n_value;
	new->type = get_type_32(list, env);
	if (new->type == 'I' || new->type == 'i')
		if (!(new->i_name = cmd_get_name(env, table + list.n_value)))
			return (error_cmd(env, cmds, new));
	new->name = cmd_get_name(env, table + list.n_un.n_strx);
	if (!new->name)
		return (error_cmd(env, cmds, new));
	cmds = new;
	return (new);
}
