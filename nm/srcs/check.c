#include "nm.h"

void	*check_addr(void *addr, size_t size, t_env *env)
{
	void	*safe;

	safe = addr;
	if (!addr)
		return (NULL);
	if (addr < env->ptr || addr >= env->ptr + env->size)
		return (NULL);
	addr += size;
	if (addr < env->ptr || addr >= env->ptr + env->size)
		return (NULL);
	return (safe);
}
