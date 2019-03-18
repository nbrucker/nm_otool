#include "nm.h"

long	reverse_long(long x)
{
	long new;

	new = (x & 0xFF) << 56;
	new += (x & 0xFF00) << 40;
	new += (x & 0xFF0000) << 24;
	new += (x & 0xFF000000) << 8;
	new += (x & 0xFF00000000) >> 8;
	new += (x & 0xFF0000000000) >> 24;
	new += (x & 0xFF000000000000) >> 40;
	new += (x & 0xFF00000000000000) >> 56;
	return (new);
}

int	reverse_int(int x)
{
	int new;

	new = (x & 0xFF) << 24;
	new += (x & 0xFF00) << 8;
	new += (x & 0xFF0000) >> 8;
	new += (x & 0xFF000000) >> 24;
	return (new);
}
