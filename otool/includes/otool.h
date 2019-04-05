#ifndef OTOOL_H
# define OTOOL_H

# include <mach-o/loader.h>
# include <mach-o/fat.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <sys/mman.h>
# include <stdlib.h>
# include <ar.h>

# define LIB_MAGIC 0x213c617263683e0a
# define LIB_CIGAM 0x0a3e686372613c21

#include <stdio.h>

typedef struct    s_env
{
  char            *file;
  void            *ptr;
  size_t          size;

  int             error;

  char            *arch;
  char            *lib_name;
  int             type;
  int			le;
}                 t_env;

typedef struct  s_arch
{
  char *name;
  cpu_type_t  cputype;
  cpu_subtype_t cpusubtype;
}               t_arch;

/*
*** check.c
*/
int	check_ar_name(void *addr, t_env *env);
void	*check_addr(void *addr, size_t size, t_env *env);

/*
*** error.c
*/
int				error_not_file(char *str);
int				error_opening_file(char *str);
int       ft_error(char *str);
void      error_cmdsize(t_env *env);
void	error_file_format(t_env *env);

/*
*** get.c
*/
int	get_ar_size(struct ar_hdr *hdr);
void	*get_ar_addr(char *name);

/*
*** handle_32.c
*/
void	print_addr_32(uint64_t value);
void	reverse_section_32(t_env *env, struct section *sec);
void	print_section_32(t_env *env, struct section *sec);
void	handle_32_segment(struct load_command *lc, t_env *env);
void	handle_32(t_env *env);

/*
*** handle_64.c
*/
void	print_addr_64(uint64_t value);
void	reverse_section_64(t_env *env, struct section_64 *sec);
void	print_section_64(t_env *env, struct section_64 *sec);
void	handle_64_segment(struct load_command *lc, t_env *env);
void	handle_64(t_env *env);

/*
*** handle_be_32.c
*/
void	handle_be_32_segment(struct load_command *lc, t_env *env);
void	handle_be_32(t_env *env);

/*
*** handle_be_64.c
*/
void	handle_be_64_segment(struct load_command *lc, t_env *env);
void	handle_be_64(t_env *env);

/*
*** handle_fat.c
*/
char  *arch_get_name(cpu_type_t cputype, cpu_subtype_t cpusubtype);
int	otool_inside(void *ptr, size_t size, char *file, char *name, int type);
void fat_print_all(t_env *env, struct fat_arch *a, uint32_t n);
void	handle_be_fat(t_env *env);
void	handle_fat(t_env *env);

/*
*** init.c
*/
t_env	*init_env(void *ptr, size_t size, char *file);

/*
*** lib.c
*/
void	handle_lib(t_env *env);

/*
*** main.c
*/
int		is_arm_ppc(char *arch);
void	(*get_function(uint32_t magic32, uint64_t magic64))(t_env*);
int	otool(void *ptr, size_t size, char *file);
int	handle_file(char *str);

/*
*** print.c
*/
void	print_before_lib(t_env *env);
void	print_before_fat(t_env *env);
void	print_before_normal(t_env *env);
void	print_before(t_env *env);
void	print_number(uint8_t value);

/*
*** reverse.c
*/
long	reverse_long(long x);
int   reverse_int(int x);

#endif
