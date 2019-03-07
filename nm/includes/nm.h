#ifndef NM_H
# define NM_H

# include <fcntl.h>
# include <sys/mman.h>

#include <stdio.h>

void	error_opening_file(char *str);
void	error_not_file(char *str);

#endif
