#include "fi.h"

unsigned short int		fi_parse_tetri_valid(t_fi *fi, unsigned short int n)
{
	int					i;
	unsigned short int	identity[19] = {51,//SQUARE
	15, 4369,//BARS
	23, 113, 71, 116,//L SIDE
	275, 547, 802, 785,//L UP
	561, 306, 54, 99,//S
	39, 114, 305, 562};//T

	while ((n & 15) == 0)
		n = n >> 4;
	while ((n & 4369) == 0)
		n = ((n & 14) >> 1) + ((n & 224) >> 1) + ((n & 3584) >> 1) + ((n & 57344) >> 1);
	i = 0;
	while (i < 19)
		if (identity[i++] == n)
			return (n);
	return (0);
}

unsigned short int		fi_parse_tetri(t_fi *fi, char *tet)
{
	unsigned short int	n;
	int					i;

	i = 0;
	n = 0;
	while (i < 20)
	{
		if (tet[i] == '#')
			n += (1 << (i - i/5));
		else if (tet[i] != '.')
			return (0);
		i += 1 + ((i + 2) % 5 == 0) * (tet[i + 1] == '\n' || tet[i + 1] == '\0');
	}

	// printf("====TET=====\n");
	// printf("%s\n", tet);
	printf("====TET=====\n");
	printf("===N: %lu\n", (long unsigned int)n);
	fi_display_short(n);
	return (fi_parse_tetri_valid(fi, n));
}

int						fi_parse_tetris(t_fi *fi)
{
	char	*tetris;
	int		i;

	i = 0;
	tetris = fi->file;
	while (*tetris && i < 26)
	{
		if ((fi->tios[i] = fi_parse_tetri(fi, tetris)) == 0)
			return (0);
		fi_display_short(fi->tios[i]);
		tetris += 20;
		tetris += (*tetris != '\0');
		i++;
	}
	printf("# Tetriminos: %d\n", i);
	return (i);
}

int						fi_parse_sizeit(int n)
{
	int					i;

	n *= 4;
	i = 2;
	while (i * i < n)
		i++;
	return (i);
}

t_fi					*fi_parse_file(int fd)
{
	t_fi				*fi;
	char				buf;
	int					i;

	fi = NULL;
	printf("WE IN\n");
	if (fi = (t_fi *)malloc(sizeof(t_fi)))
	{
		fi->file = ft_strnew(545);
		fi->fd = fd;
		i = 0;
		while (read(fd, &buf, 1) > 0)
			fi->file[i++] = buf;
		fi->file[i] = '\0';
		close(fd);
		if ((fi->ntios = fi_parse_tetris(fi)) == 0)
			return (NULL);
		fi->msize = fi_init_sizeit(fi->ntios);
		fi->size = fi->msize - 1;
		fi->max = fi->size * fi->size;
		fi->m = fi_init_smap(fi->msize);
	}
	printf("WE OUT\n");
	return (fi);
}