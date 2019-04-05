#include "otool.h"

int		check_ar_name(void *addr, t_env *env)
{
	char *name;

	name = (char*)addr;
	while (check_addr((void*)name, 1, env) && name[0])
		name++;
	while (check_addr((void*)name, 1, env) && !name[0])
		name++;
	return (env->error);
}

void	*check_addr(void *addr, size_t size, t_env *env)
{
	void	*safe;

	safe = addr;
	if (!addr)
	{
		env->error = 1;
		return (NULL);
	}
	if (addr < env->ptr || addr >= env->ptr + env->size)
	{
		env->error = 1;
		return (NULL);
	}
	addr += size;
	if (addr < env->ptr || addr > env->ptr + env->size)
	{
		env->error = 1;
		return (NULL);
	}
	return (safe);
}
