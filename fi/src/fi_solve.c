#include "fi.h"

static int					fi_solve_place_fit(unsigned short int x, int y, unsigned short int t, t_fi *fi)
{
	//ALIGN ALL ON LEFT FOUR BITS (LOWER ONES)
	if (
		((t & NIB_000F) & ((fi->m[y] >> x) & NIB_000F)) == 0 &&
		(((t & NIB_00F0) >> 4) & ((fi->m[y + 1] >> x) & NIB_000F)) == 0 &&
		(((t & NIB_0F00) >> 8) & ((fi->m[y + 2] >> x) & NIB_000F)) == 0 &&
		(((t & NIB_F000) >> 12) & ((fi->m[y + 3] >> x) & NIB_000F)) == 0
		)
		return (1);
	else
		return (0);
}

// static int					fi_solve_place_fit(unsigned short int x, int y, unsigned short int t)
// {
// 	if (t &&
// 	(((t & NIB_000F) << x) & ((NIB_000F << x) & fi->m[y])) == 0 &&
// 	((((t & NIB_00F0) >> 4) << x) & ((NIB_000F << x) & fi->m[y + 1])) == 0 &&
// 	((((t & NIB_0F00) >> 8) << x) & ((NIB_000F << x) & fi->m[y + 2])) == 0 &&
// 	((((t & NIB_F000) >> 12) << x) & ((NIB_000F << x) & fi->m[y + 3])) == 0)
// 		return (1);
// 	else
// 		return (0);
// }

static void					fi_solve_remove_at(unsigned short int x, int y, unsigned short int t, t_fi *fi)
{
	fi->m[y] &= ~((t & NIB_000F) << x);
	fi->m[y + 1] &= ~(((t & NIB_00F0) >> 4) << x);
	fi->m[y + 2] &= ~(((t & NIB_0F00) >> 8) << x);
	fi->m[y + 3] &= ~(((t & NIB_F000) >> 12) << x);

}

static void					fi_solve_place_at(unsigned short int x, int y, unsigned short int t, t_fi *fi)
{
	fi->m[y] |= ((t & NIB_000F) << x);
	fi->m[y + 1] |= (((t & NIB_00F0) >> 4) << x);
	fi->m[y + 2] |= (((t & NIB_0F00) >> 8) << x);
	fi->m[y + 3] |= (((t & NIB_F000) >> 12) << x);

}

static int					fi_solve_place(int i, int k, int size, int max, t_fi *fi)
{
	while (i < max)
	{
		// if (fi->tios[k] != 15 && ((i / size) == (size - 1)))
		// 	return (-1);
		if (fi_solve_place_fit(i % size, i / size, fi->tios[k], fi))
			return (i);
		i++;
	}
	return (-1);
}

void						fi_solve_curse(int index, int k, int *done, t_fi *fi)
{
	if (*done)
		return ;
	if (k < fi->ntios)
	{
		while ((index = fi_solve_place(index, k, fi->size, fi->max, fi)) != -1)
		{
			fi_solve_place_at(index % fi->size, index / fi->size, fi->tios[k], fi);
			// fi_display_map(fi);
			// fi_display_smap(fi);
			// usleep(170000);
			fi->idc[k] = index;
			fi_solve_curse(0, k + 1, done, fi);
			if (*done)
				return ;
			fi_solve_remove_at(index % fi->size, index / fi->size, fi->tios[k], fi);
			index++;
		}
		return;
	}
	*done = 1;
	return ;
}