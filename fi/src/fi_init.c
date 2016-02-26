#include "fi.h"

int						fi_init_sizeit(int n)
{
	int					i;

	n *= 4;
	i = 2;
	while (i * i < n)
		i++;
	return (i);
}

unsigned short int		*fi_init_smap(int min)
{
	unsigned short int	*m;
	int					i;

	m = NULL;
	if ((m = (unsigned short int *)malloc(sizeof(unsigned short int) * (min + 3))) != NULL)
	{
		i = 0;
		while (i < 16)
		{
			m[0] += (i < min) ? 0 : (BIT_R << i);
			i++;
		}
		i = 1;
		while (i < min + 3)
		{
			m[i] = (i < min) ? m[0] : BIT_FULL;
			i++;
		}
	}
	return (m);
}
