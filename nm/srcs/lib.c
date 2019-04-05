#include "nm.h"

int	nm_inside(void *ptr, size_t size, char *file, char *name, int type)
{
	void (*f)(t_env*);
	t_env			*env;
	int				ret;

	if (!(env = init_env(ptr, size, file)))
		return (1);
	if (size < 8)
		return (1);
	f = get_function(((uint32_t*)ptr)[0], ((uint64_t*)ptr)[0]);
	if (f)
		f(env);
	else
		error_file_format(env);
	if (type == 1)
		print_ar_name(env, name);
	else if (type == 2)
		print_fat_arch(env, name);
	print_cmds(env);
	free_cmds(env->cmd);
	ret = env->error;
	free(env);
	return (ret);
}

void	handle_lib(t_env *env)
{
	struct ar_hdr *hdr;
	void *ptr;
	int size;
	char *name;
	void *new;

	ptr = env->ptr + 8;
	if (!check_addr(ptr, sizeof(struct ar_hdr), env))
		return ;
	hdr = (struct ar_hdr*)ptr;
	size = get_ar_size(hdr);
	ptr += sizeof(struct ar_hdr) + size;
	while (ptr < env->ptr + env->size && env->error == 0 && check_addr(ptr, sizeof(struct ar_hdr), env))
	{
		hdr = (struct ar_hdr*)ptr;
		size = get_ar_size(hdr);
		if (check_ar_name(ptr + sizeof(struct ar_hdr), env))
			return ;
		name = ptr + sizeof(struct ar_hdr);
		new = get_ar_addr(name);
		env->error = nm_inside(new, size - (new - (void*)name), env->file, name, 1);
		ptr += sizeof(struct ar_hdr) + size;
	}
}
