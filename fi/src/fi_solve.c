#include "fi.h"


int						fi_solve_place_fit(t_fi *fi, unsigned short int x, int y, unsigned short int t)
{
	// printf("-----------------------\n");
	// fi_display_smap(fi);
	// printf("-----------------------\n");
	// usleep(1000000);
	if (t &&
	(((t & NIB_000F) << x) & ((NIB_000F << x) & fi->m[y])) == 0 &&
	((((t & NIB_00F0) >> 4) << x) & ((NIB_000F << x) & fi->m[y + 1])) == 0 &&
	((((t & NIB_0F00) >> 8) << x) & ((NIB_000F << x) & fi->m[y + 2])) == 0 &&
	((((t & NIB_F000) >> 12) << x) & ((NIB_000F << x) & fi->m[y + 3])) == 0
	)
	{
		// printf("FIT!\n");
		// fi_display_short(t);
		// 	fi_display_smap(fi);

		// printf("====\n");
		// fi->m[y] |= ((t & NIB_000F) << x);
		// fi->m[y + 1] |= (((t & NIB_00F0) >> 4) << x);
		// fi->m[y + 2] |= (((t & NIB_0F00) >> 8) << x);
		// fi->m[y + 3] |= (((t & NIB_F000) >> 12) << x);
	}
	else
		return (0);
	return (1);
}

void						fi_solve_remove_at(t_fi *fi, unsigned short int x, int y, unsigned short int t)
{
	fi->m[y] &= ~((t & NIB_000F) << x);
	fi->m[y + 1] &= ~(((t & NIB_00F0) >> 4) << x);
	fi->m[y + 2] &= ~(((t & NIB_0F00) >> 8) << x);
	fi->m[y + 3] &= ~(((t & NIB_F000) >> 12) << x);

}


void						fi_solve_place_at(t_fi *fi, unsigned short int x, int y, unsigned short int t)
{
	fi->m[y] |= ((t & NIB_000F) << x);
	fi->m[y + 1] |= (((t & NIB_00F0) >> 4) << x);
	fi->m[y + 2] |= (((t & NIB_0F00) >> 8) << x);
	fi->m[y + 3] |= (((t & NIB_F000) >> 12) << x);

}

int							fi_solve_place(t_fi *fi, int k, int i, int size, int max)
{
	// int					i;
	// int					size;
	// int					max;

	// i = start;
	// size = fi->msize - 1;
	// max = size * size;
	while (i < max)
	{
		if (fi_solve_place_fit(fi, i % size, i / size, fi->tios[k]))
			return (i);
		i++;
	}
	// printf("===========NO SPACE===========\n");
	return (-1);
}

int						fi_solve_index(t_fi *fi, int start)
{
	int					i;
	int					j;

	i = start / fi->msize;
	j = start % fi->msize;
	while (i < fi->msize)
	{	
		while (j < fi->msize)
		{
			if ((BIT_R << j) & ~fi->m[i])
				return (i * fi->msize + j);
			j++;
		}
		j = 0;
		i++;
	}
	return (-1);
}

void					fi_solve_curse(t_fi *fi, int k, int index, int *done, int *count)
{
	// fi_display_smap(fi);
	(*count)++;
	if (*done)
		return ;
	if (k < fi->ntios)
	{
		while ((index = fi_solve_place(fi, k, index, fi->size, fi->max)) != -1)
		{
			// printf("K %d I %d\n", k, index);
			fi_solve_place_at(fi, index % fi->size, index / fi->size, fi->tios[k]);
			// fi_display_smap(fi);
			// usleep(40000);
			fi_solve_curse(fi, k + 1, 0, done, count);
			if (*done)
				return ;
			fi_solve_remove_at(fi, index % fi->size, index / fi->size, fi->tios[k]);
			index++;
		}

		return;
	}
	fi_display_smap(fi);
	*done = 1;
	printf("BINGO\n");
	// exit(0);
	return ;
}

// void					fi_solve_curse(t_fi *fi, int k, int index)
// {
// 	// fi_display_smap(fi);
// 	if (fi_solve_place(fi, k, index) != -1)
// 	{
// 		while (k < fi->ntios)
// 		{
// 			k++;
// 			fi_solve_curse(fi, k, index);
// 		}
// 	}
// 	printf("STUCK BACK\n");
// 	return ;
// }


// void					fi_solve_curse(t_fi *fi, int k, int index)
// {
// 	if (k < fi->ntios)
// 	{
// 		fi_display_smap(fi);
// 		usleep(100000);
// 		if (fi_solve_place(fi, k, index) != -1)
// 			fi_solve_curse(fi, ++k, 0);
// 		// else
// 		{
// 			// index++;
// 			// k++;
			
// 		}
// 			// return ;
// 		// else
// 		// 	fi_solve_curse(fi, --k, ++index);
// 		// index = fi_solve_index(fi, ++index);
// 	}
// }

void					fi_solve_fillit(t_fi *fi)
{
	int					done;
	int					count;

	done = 0;
	count = 0;
	printf("SOLVER IN\n");
	for (int k = 0;fi->tios[k] != 0;k++)
		printf("TET NUM -> %lu\n", (unsigned long int)fi->tios[k]);
	printf("# Pieces: %d\n", fi->ntios);
	printf("=>MIN SIZE THEORETICAL=> %d\n", fi->msize);
	// fi_solve_curse(fi, 0, 0);
	// for (int k = 0;k < fi->ntios;k++)
	// {
	// 	printf("====MAP====\n");
	// 	fi_display_smap(fi);
	// 	printf("====MAP====\n");
	// 	usleep(1000000);
	// 	fi_solve_place(fi, k, index);
	// 	index = fi_solve_index(fi, index);
	// 	printf("INDEX= %d\n", index);
	// }
		while (!done)
		{
			fi_display_smap(fi);
			fi_solve_curse(fi, 0, 0, &done, &count);
			if (!done)
			{
				fi->msize++;
				fi->size++;
				fi->max = fi->size * fi->size;
				fi->m = fi_init_smap(fi->msize);
			}
		}

		printf("COUNT: %d\n", count);
	printf("====FILE====\n");
	// printf("%s\n", fi->file);
	return ;
}