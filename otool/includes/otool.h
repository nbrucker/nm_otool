#ifndef OTOOL_H
# define OTOOL_H

# include <mach-o/loader.h>
# include <mach-o/fat.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <sys/mman.h>
# include <stdlib.h>

#include <stdio.h>

typedef struct    s_env
{
  char            *file;
  void            *ptr;
  size_t          size;

  int             error;

  char            *arch;
  int             type;
}                 t_env;

typedef struct  s_arch
{
  char *name;
  cpu_type_t  cputype;
  cpu_subtype_t cpusubtype;
}               t_arch;

char  *arch_get_name(cpu_type_t cputype, cpu_subtype_t cpusubtype);
t_env	*init_env(void *ptr, size_t size, char *file);
void	print_addr_64(uint64_t value);
void	print_addr_32(uint64_t value);
void	(*get_function(uint32_t magic32, uint64_t magic64))(t_env*);
void	print_before(t_env *env);
int	otool(void *ptr, size_t size, char *file);
void	handle_64(t_env *env);
void	handle_32(t_env *env);
void	handle_be_64(t_env *env);
void	handle_be_32(t_env *env);
void	handle_be_fat(t_env *env);
void	handle_fat(t_env *env);
void	print_number(uint8_t value);
void	print_section_32(t_env *env, struct section *sec);
void	print_section_64(t_env *env, struct section_64 *sec);

/*
*** check.c
*/
void	*check_addr(void *addr, size_t size, t_env *env);

/*
*** error.c
*/
int				error_not_file(char *str);
int				error_opening_file(char *str);
int       ft_error(char *str);
void      error_cmdsize(t_env *env);

/*
*** reverse.c
*/
long	reverse_long(long x);
int   reverse_int(int x);

#endif
