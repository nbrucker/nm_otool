#ifndef NM_H
# define NM_H

/*
*** Deps external
*/
# include <fcntl.h>
# include <mach-o/loader.h>
# include <mach-o/nlist.h>
# include <sys/stat.h>
# include <sys/mman.h>

#include <stdio.h>

typedef struct    s_env
{
  void            *ptr;

  int             sec_index;
  int             t_index;
  int             d_index;
  int             b_index;
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
*** create.c
*/
t_cmd	*create_cmd(t_cmd *cmds);

/*
*** get.c
*/
char	get_type_64(struct nlist_64 list, t_env *env);
char	get_section_64(unsigned char s, t_env *env);
t_cmd	*get_first_cmd(t_cmd *cmds);

/*
*** handle_64.c
*/
void	handle_64(t_env *env);
void	handle_64_segment(struct load_command *lc, t_env *env);
void	handle_64_symtab(struct load_command *lc, t_env *env);

/*
*** init.c
*/
t_env	*init_env(void);

/*
*** main.c
*/
void	nm(void *ptr);
void	handle_file(char *str, int ac);

/*
*** error.c
*/
void				error_not_file(char *str);
void				error_opening_file(char *str);

/*
*** print.c
*/
void	print_cmds(t_cmd *cmds);
void	print_file_name(char *str);
void	print_value(uint64_t value);

/*
*** treat_cmd.c
*/
void	sort_cmds(t_cmd *cmds);
void	swap_cmd(t_cmd *a, t_cmd *b);

#endif
