#include "fi.h"

unsigned long int		fi_stl(unsigned short int n)
{
	unsigned long int	l;

	l = 0;
	l = (((unsigned long int)n) & 15);// |
	 // ((((unsigned long int)n) & 240) << 4);// | 
	 // ((unsigned long int)(n & 3840) << 8) |
	 //  ((unsigned long int)(n & 61440) << 12);
	printf("VAL: %lu\n", l);
	return(l);
}

void					fi_display_short_inline(unsigned short int n)
{
	int					i;

	i = 0;
	while (i < 16)
		ft_putstr((n & (1 << i++)) ? "\033[44m1\x1B[0m" : "0");
	ft_putchar('\n');
}

void					fi_display_short(unsigned short int n)
{
	int					i;

	i = 0;
	while (i < 16)
	{
		ft_putstr((n & (1 << i++)) ? "\033[44m1\x1B[0m" : "0");
		if (i % 4 == 0)
			ft_putchar('\n');
	}
	ft_putchar('\n');
}

void					fi_display_smap(t_fi *fi)
{
	int					i;

	i = 0;
	printf("______________________________________\n");
	while (i < fi->msize + 3)
		fi_display_short_inline(fi->m[i++]);
	printf("--------------------------------------\n");
}