#include "fi.h"

static char					**fi_display_load_map(char **tab, t_fi *fi)
{
	int					i;
	int					k;
	int					x;
	int					y;

	k = 0;
	while (k < fi->ntios)
	{
		i = 0;
		while (i < 16)
		{
			y = (fi->idc[k] / fi->msize) + (i / 4);
			x = (fi->idc[k] % fi->msize) + (i % 4);
			if (((1 << i) & fi->tios[k]) != 0)
				tab[y][x] = k + 'A';
			i++;
		}
		k++;
	}
	return (tab);
}


void					fi_display_short_inline(unsigned short int n)
{
	int					i;

	i = 0;
	while (i < 16)
		ft_putstr((n & (1 << i++)) ? "\033[44m1\x1B[0m" : "0");
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

static char					**fi_display_init_map(int size)
{
	int					i;
	int					j;
	char				**tab;

	if ((tab = (char **)malloc(sizeof(char *) * (size))) != NULL)
	{
		i = 0;
		while (i < size)
		{
			if ((tab[i] = (char *)malloc(sizeof(char) * (size + 1))) != NULL)
			{
				j = 0;
				while (j < size)
					tab[i][j++] = '.';
				tab[size] = '\0';
				i++;			
			}
		}
	}
	return (tab);
}

void					fi_display_map(t_fi *fi)
{
	int					i;
	char				**tab;

	i = 0;
	tab = fi_display_init_map(fi->msize);
	tab = fi_display_load_map(tab, fi);
	while (i < fi->msize)
		ft_putendl(tab[i++]);
	// ft_putendl("================");
}