#ifndef NM_H
# define NM_H

/*
*** Deps external
*/
# include <fcntl.h>
# include <mach-o/loader.h>
# include <mach-o/nlist.h>
# include <mach-o/fat.h>
# include <mach-o/arch.h>
# include <ar.h>
# include <mach/machine.h>
# include <sys/stat.h>
# include <sys/mman.h>
# include <stdlib.h>

# define LIB_MAGIC 0x213c617263683e0a
# define LIB_CIGAM 0x0a3e686372613c21

#include <stdio.h>

typedef struct    s_env
{
  char            *file;
  void            *ptr;
  size_t          size;
  int             type;

  int             sec_index;
  int             t_index;
  int             d_index;
  int             b_index;

  int             error;

  struct s_cmd    *cmd;
}                 t_env;

typedef struct		s_cmd
{
	char			*name;
	uint64_t		value;
	char          type;
  char *i_name;
  int ind;
	struct s_cmd	*next;
	struct s_cmd	*previous;
}					t_cmd;

typedef struct  s_arch
{
  char *name;
  cpu_type_t  cputype;
  cpu_subtype_t cpusubtype;
}               t_arch;

void	(*get_function(uint32_t magic32, uint64_t magic64))(t_env*);
int	nm_inside(void *ptr, size_t size, char *file, char *name, int type);

/*
*** check.c
*/
int	check_ar_name(void *addr, t_env *env);
void	*check_addr(void *addr, size_t size, t_env *env);

/*
*** create.c
*/
t_cmd	*create_cmd(t_cmd *cmds, char *table, struct nlist_64 list, t_env *env);
t_cmd	*create_cmd_32(t_cmd *cmds, char *table, struct nlist list, t_env *env);

/*
*** error.c
*/
int				error_not_file(char *str);
int				error_opening_file(char *str);
int       ft_error(char *str);
void      error_cmdsize(t_env *env);
void      error_file_format(t_env *env);

/*
*** free.c
*/
void  free_cmds(t_cmd *cmd);

/*
*** get_ar.c
*/
int   get_ar_size(struct ar_hdr *hdr);
void	*get_ar_addr(char *name);
/*
*** get.c
*/
char	get_type_32(struct nlist list, t_env *env);
char	get_type_64(struct nlist_64 list, t_env *env);
char	get_section_64(unsigned char s, t_env *env);
t_cmd	*get_first_cmd(t_cmd *cmds);
t_cmd	*get_last_cmd(t_cmd *cmds);

/*
*** handle_32.c
*/
void	handle_32(t_env *env);
void  h32_seg(struct mach_header *h, struct load_command *lc, t_env *env);
void  h32_sym(struct mach_header *h, struct load_command *lc, t_env *env);
void	handle_32_segment(struct load_command *lc, t_env *env);
void	handle_32_symtab(struct load_command *lc, t_env *env);

/*
*** handle_64.c
*/
void	handle_64(t_env *env);
void	h64_seg(struct mach_header_64 *h, struct load_command *lc, t_env *env);
void	h64_sym(struct mach_header_64 *h, struct load_command *lc, t_env *env);
void	handle_64_segment(struct load_command *lc, t_env *env);
void	handle_64_symtab(struct load_command *lc, t_env *env);

/*
*** handle_be_32.c
*/
void	handle_be_32(t_env *env);
void	handle_be_32_segment(struct load_command *lc, t_env *env);
void	handle_be_32_symtab(struct load_command *lc, t_env *env);

/*
*** handle_be_64.c
*/
void	handle_be_64(t_env *env);
void	handle_be_64_segment(struct load_command *lc, t_env *env);
void	handle_be_64_symtab(struct load_command *lc, t_env *env);

/*
*** handle_fat.c
*/
void	handle_be_fat(t_env *env);
void	handle_fat(t_env *env);

/*
*** init.c
*/
t_env	*init_env(void *ptr, size_t size, char *file);

/*
*** lib.c
*/
int   nm_inside(void *ptr, size_t size, char *file, char *name, int type);
void	handle_lib(t_env *env);

/*
*** main.c
*/
int	nm(void *ptr, size_t size, char *file);
int	handle_file(char *str, int ac);
void	(*get_function(uint32_t magic32, uint64_t magic64))(t_env*);
void	set_error(t_env *env);

/*
*** print_ar.c
*/
void	print_fat_arch(t_env *env, char *name);
void	print_ar_name(t_env *env, char *name);

/*
*** print.c
*/
void	print_cmds(t_env *env);
void	print_file_name(char *str);
void	print_value_64(uint64_t value, char type, int ind);
void	print_value_32(uint64_t value, char type, int ind);

/*
*** reverse.c
*/
long	reverse_long(long x);
int   reverse_int(int x);

/*
*** treat_cmd.c
*/
void	set_ind_cmd_name(t_cmd *cmd);
void	sort_cmds(t_cmd *cmd);
void	swap_cmd(t_cmd *a, t_cmd *b);

#endif
