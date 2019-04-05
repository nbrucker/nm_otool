#include "otool.h"
#include "libft.h"

void	lib_call_otool(char *ptr, int size, t_env *env)
{
	char	*name;
	void	*new;

	name = ptr + sizeof(struct ar_hdr);
	new = get_ar_addr(name);
	env->error = otool_inside(new, size - (new - (void*)name), env->file, name);
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
