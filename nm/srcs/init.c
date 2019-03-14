#include "libft.h"
#include "nm.h"

t_env	*init_env(void)
{
	t_env			*env;

	if (!(env = (t_env*)malloc(sizeof(t_env))))
		return (NULL);
	ft_bzero((void*)env, sizeof(t_env));
	env->sec_index = 1;
	return (env);
}
