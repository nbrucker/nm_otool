#include "nm.h"

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
	if (addr < env->ptr || addr >= env->ptr + env->size)
	{
		env->error = 1;
		return (NULL);
	}
	return (safe);
}
