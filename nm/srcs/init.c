#include "libft.h"
#include "nm.h"

t_env	*init_env(void *ptr, size_t size, char *file)
{
	t_env			*env;

	if (!(env = (t_env*)malloc(sizeof(t_env))))
		return (NULL);
	ft_bzero((void*)env, sizeof(t_env));
	env->sec_index = 1;
	env->ptr = ptr;
	env->size = size;
	env->file = file;
	return (env);
}
