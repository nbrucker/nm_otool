#include "otool.h"
#include "libft.h"

int		otool_inside_lib(void *ptr, size_t size, char *file, char *name)
{
	void	(*f)(t_env*);
	t_env	*env;
	int		ret;

	if (!(env = init_env(ptr, size, file)))
		return (1);
	if (size < 8)
		return (1);
	env->type = 2;
	env->lib_name = name;
	f = get_function(((uint32_t*)ptr)[0], ((uint64_t*)ptr)[0]);
	if (f)
		f(env);
	ret = env->error;
	if (env->arch)
		free(env->arch);
	free(env);
	return (ret);
}

void	lib_call_otool(char *ptr, int size, t_env *env)
{
	char	*name;
	void	*new;

	name = ptr + sizeof(struct ar_hdr);
	new = get_ar_addr(name);
	env->error = otool_inside_lib(new, size - (new - (void*)name),
		env->file, name);
}

void	handle_lib(t_env *env)
{
	struct ar_hdr	*hdr;
	void			*ptr;
	int				size;

	ptr = env->ptr + 8;
	if (!check_addr(ptr, sizeof(struct ar_hdr), env))
		return ;
	ft_putstr("Archive : ");
	ft_putendl(env->file);
	hdr = (struct ar_hdr*)ptr;
	size = get_ar_size(hdr);
	ptr += sizeof(struct ar_hdr) + size;
	while (ptr < env->ptr + env->size && env->error == 0
		&& check_addr(ptr, sizeof(struct ar_hdr), env))
	{
		hdr = (struct ar_hdr*)ptr;
		size = get_ar_size(hdr);
		if (check_ar_name(ptr + sizeof(struct ar_hdr), env))
			return ;
		lib_call_otool(ptr, size, env);
		ptr += sizeof(struct ar_hdr) + size;
	}
}
