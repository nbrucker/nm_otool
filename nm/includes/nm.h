#ifndef NM_H
# define NM_H

/*
*** Deps external
*/
# include <fcntl.h>
# include <mach-o/loader.h>
# include <mach-o/nlist.h>
# include <mach-o/fat.h>
# include <mach/machine.h>
# include <sys/stat.h>
# include <sys/mman.h>
# include <stdlib.h>

#include <stdio.h>

typedef struct    s_env
{
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
	struct s_cmd	*next;
	struct s_cmd	*previous;
}					t_cmd;

/*
*** check.c
*/
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

/*
*** free.c
*/
void  free_cmds(t_cmd *cmd);

/*
*** get.c
*/
char	get_type_32(struct nlist list, t_env *env);
char	get_type_64(struct nlist_64 list, t_env *env);
char	get_section_64(unsigned char s, t_env *env);
t_cmd	*get_first_cmd(t_cmd *cmds);

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
t_env	*init_env(void *ptr, size_t size);

/*
*** main.c
*/
int	nm(void *ptr, size_t size);
int	handle_file(char *str, int ac);

/*
*** print.c
*/
void	print_cmds(t_env *env);
void	print_file_name(char *str);
void	print_value_64(uint64_t value, char type);
void	print_value_32(uint64_t value, char type);

/*
*** reverse.c
*/
long	reverse_long(long x);
int   reverse_int(int x);

/*
*** treat_cmd.c
*/
void	sort_cmds(t_cmd *cmd);
void	swap_cmd(t_cmd *a, t_cmd *b);

#endif
