#ifndef NM_H
# define NM_H

# include <fcntl.h>
# include <sys/mman.h>
# include <sys/stat.h>
# include <mach-o/loader.h>
# include <mach-o/nlist.h>

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

void				error_opening_file(char *str);
void				error_not_file(char *str);

#endif
