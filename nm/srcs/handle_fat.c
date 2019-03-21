#include "nm.h"
#include "libft.h"

void	handle_be_fat(t_env *env)
{
	struct fat_header	*h;
	struct fat_arch		*a;
	uint32_t i;

	if (!(h = (struct fat_header*)check_addr(env->ptr,
		sizeof(struct fat_header), env)))
		return ;
	h->nfat_arch = reverse_int(h->nfat_arch);
	i = 0;
	a = env->ptr + sizeof(struct fat_header);
	while (i < h->nfat_arch && check_addr((void*)a,
		sizeof(struct fat_arch), env) && env->error == 0)
	{
		a->offset = reverse_int(a->offset);
		a->size = reverse_int(a->size);
		a++;
		i++;
	}
	if (env->error == 0)
		handle_fat(env);
}

void	handle_fat(t_env *env)
{
	struct fat_header	*h;
	struct fat_arch		*a;
	struct fat_arch		*safe;
	uint32_t i;

	if (!(h = (struct fat_header*)check_addr(env->ptr,
		sizeof(struct fat_header), env)))
		return ;
	i = 0;
	safe = NULL;
	a = env->ptr + sizeof(struct fat_header);
	while (i < h->nfat_arch && check_addr((void*)a,
		sizeof(struct fat_arch), env) && env->error == 0)
	{
		if (check_addr(env->ptr + a->offset, a->size, env))
			safe = a;
		else
			return (ft_putendl("wrong offset/size"));
		a++;
		i++;
	}
	if (env->error == 0 && safe)
		env->error = nm(env->ptr + safe->offset, safe->size);
}
