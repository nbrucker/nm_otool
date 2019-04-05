#include "otool.h"
#include "libft.h"

static const struct s_arch archs[] = {
	{ "any", CPU_TYPE_ANY, CPU_SUBTYPE_MULTIPLE},
	{ "little", CPU_TYPE_ANY, CPU_SUBTYPE_LITTLE_ENDIAN},
	{ "big", CPU_TYPE_ANY, CPU_SUBTYPE_BIG_ENDIAN},
	{ "ppc64", CPU_TYPE_POWERPC64, CPU_SUBTYPE_POWERPC_ALL},
	{ "x86_64", CPU_TYPE_X86_64, CPU_SUBTYPE_X86_64_ALL},
	{ "ppc970-64", CPU_TYPE_POWERPC64, CPU_SUBTYPE_POWERPC_970},
	{ "ppc", CPU_TYPE_POWERPC, CPU_SUBTYPE_POWERPC_ALL},
	{ "i386", CPU_TYPE_I386, CPU_SUBTYPE_I386_ALL},
	{ "m68k", CPU_TYPE_MC680x0, CPU_SUBTYPE_MC680x0_ALL},
	{ "hppa", CPU_TYPE_HPPA, CPU_SUBTYPE_HPPA_ALL},
	{ "sparc", CPU_TYPE_SPARC, CPU_SUBTYPE_SPARC_ALL},
	{ "m88k", CPU_TYPE_MC88000, CPU_SUBTYPE_MC88000_ALL},
	{ "i860", CPU_TYPE_I860, CPU_SUBTYPE_I860_ALL},
	{ "arm", CPU_TYPE_ARM, CPU_SUBTYPE_ARM_ALL},
	{ "ppc601", CPU_TYPE_POWERPC, CPU_SUBTYPE_POWERPC_601},
	{ "ppc603", CPU_TYPE_POWERPC, CPU_SUBTYPE_POWERPC_603},
	{ "ppc603e", CPU_TYPE_POWERPC, CPU_SUBTYPE_POWERPC_603e},
	{ "ppc603ev", CPU_TYPE_POWERPC, CPU_SUBTYPE_POWERPC_603ev},
	{ "ppc604", CPU_TYPE_POWERPC, CPU_SUBTYPE_POWERPC_604},
	{ "ppc604e", CPU_TYPE_POWERPC, CPU_SUBTYPE_POWERPC_604e},
	{ "ppc750", CPU_TYPE_POWERPC, CPU_SUBTYPE_POWERPC_750},
	{ "ppc7400", CPU_TYPE_POWERPC, CPU_SUBTYPE_POWERPC_7400},
	{ "ppc7450", CPU_TYPE_POWERPC, CPU_SUBTYPE_POWERPC_7450},
	{ "ppc970", CPU_TYPE_POWERPC, CPU_SUBTYPE_POWERPC_970},
	{ "i486", CPU_TYPE_I386, CPU_SUBTYPE_486},
	{ "i486SX", CPU_TYPE_I386, CPU_SUBTYPE_486SX},
	{ "pentium", CPU_TYPE_I386, CPU_SUBTYPE_PENT},
	{ "i586", CPU_TYPE_I386, CPU_SUBTYPE_586},
	{ "pentpro", CPU_TYPE_I386, CPU_SUBTYPE_PENTPRO},
	{ "i686", CPU_TYPE_I386, CPU_SUBTYPE_PENTPRO},
	{ "pentIIm3", CPU_TYPE_I386, CPU_SUBTYPE_PENTII_M3},
	{ "pentIIm5", CPU_TYPE_I386, CPU_SUBTYPE_PENTII_M5},
	{ "pentium4", CPU_TYPE_I386, CPU_SUBTYPE_PENTIUM_4},
	{ "m68030", CPU_TYPE_MC680x0, CPU_SUBTYPE_MC68030_ONLY},
	{ "m68040", CPU_TYPE_MC680x0, CPU_SUBTYPE_MC68040},
	{ "hppa7100LC", CPU_TYPE_HPPA, CPU_SUBTYPE_HPPA_7100LC},
	{ "arm64", CPU_TYPE_ARM64, CPU_SUBTYPE_ARM64_ALL},
	{ "armv4t", CPU_TYPE_ARM, CPU_SUBTYPE_ARM_V4T},
	{ "armv5", CPU_TYPE_ARM, CPU_SUBTYPE_ARM_V5TEJ},
	{ "xscale", CPU_TYPE_ARM, CPU_SUBTYPE_ARM_XSCALE},
	{ "armv6", CPU_TYPE_ARM, CPU_SUBTYPE_ARM_V6},
	{ "armv7", CPU_TYPE_ARM, CPU_SUBTYPE_ARM_V7},
	{ "armv7f", CPU_TYPE_ARM, CPU_SUBTYPE_ARM_V7F},
	{ "armv7k", CPU_TYPE_ARM, CPU_SUBTYPE_ARM_V7K},
	{ "armv7s", CPU_TYPE_ARM, CPU_SUBTYPE_ARM_V7S},
	{ NULL, 0, 0}
};

char	*arch_get_name(cpu_type_t cputype, cpu_subtype_t cpusubtype)
{
	int i;

	i = 0;
	while (archs[i].name)
	{
		if (cputype == archs[i].cputype && (cpusubtype
			& ~CPU_SUBTYPE_MASK) == (archs[i].cpusubtype & ~CPU_SUBTYPE_MASK))
			return (archs[i].name);
		i++;
	}
	return ("");
}

int		otool_inside_fat(void *ptr, size_t size, char *file, char *name)
{
	void	(*f)(t_env*);
	t_env	*env;
	int		ret;

	if (!(env = init_env(ptr, size, file)))
		return (1);
	if (size < 8)
		return (1);
	env->type = 1;
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

void	fat_print_all(t_env *env, struct fat_arch *a, uint32_t n)
{
	uint32_t i;

	i = 0;
	while (i < n && check_addr((void*)a,
		sizeof(struct fat_arch), env) && env->error == 0)
	{
		if (check_addr(env->ptr + a->offset, a->size, env))
			env->error = otool_inside_fat(env->ptr + a->offset, a->size,
				env->file, NULL);
		a++;
		i++;
	}
}

void	handle_be_fat(t_env *env)
{
	struct fat_header	*h;
	struct fat_arch		*a;
	uint32_t			i;

	if (!(h = (struct fat_header*)check_addr(env->ptr,
		sizeof(struct fat_header), env)))
		return ;
	h->nfat_arch = reverse_int(h->nfat_arch);
	i = 0;
	a = env->ptr + sizeof(struct fat_header);
	while (i < h->nfat_arch && check_addr((void*)a,
		sizeof(struct fat_arch), env) && env->error == 0)
	{
		a->cputype = reverse_int(a->cputype);
		a->cpusubtype = reverse_int(a->cpusubtype);
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
	uint32_t			i;

	if (!(h = (struct fat_header*)check_addr(env->ptr,
		sizeof(struct fat_header), env)))
		return ;
	safe = NULL;
	i = 0;
	a = env->ptr + sizeof(struct fat_header);
	while (i < h->nfat_arch && check_addr((void*)a,
		sizeof(struct fat_arch), env) && env->error == 0)
	{
		if (!check_addr(env->ptr + a->offset, a->size, env))
			return (ft_putendl("offset plus size too big"));
		if (ft_strcmp(arch_get_name(a->cputype, a->cpusubtype), "x86_64") == 0)
			safe = a;
		a++;
		i++;
	}
	if (env->error == 0 && safe)
		env->error = otool(env->ptr + safe->offset, safe->size, env->file);
	else if (env->error == 0 && !safe)
		fat_print_all(env, env->ptr + sizeof(struct fat_header), h->nfat_arch);
}
