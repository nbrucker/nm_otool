#include "nm.h"
#include "libft.h"

int	get_ar_size(struct ar_hdr *hdr)
{
	char tmp[11];

	ft_bzero(tmp, 11);
	ft_memcpy(tmp, hdr->ar_size, 10);
	return (ft_atoi(tmp));
}

void	*get_ar_addr(char *name)
{
	name += ft_strlen(name);
	while (!name[0])
		name++;
	return ((void*)name);
}
