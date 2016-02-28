#include "fi.h"

static void		fi_error(void)
{
	ft_putstr("error\n");
	exit(0);
}

static void					fi_solve_fillit(t_fi *fi)
{
	int					done;

	done = 0;
	while (!done)
	{
		fi_solve_curse(0, 0, &done, fi);
		if (!done)
		{
			fi->msize++;
			fi->size++;
			fi->max = fi->size * fi->size - 3;
			fi->m = fi_init_smap(fi->msize);
		}
	}
	fi_display_map(fi);
}

int				main(int ac, char **av)
{
	int			fd;
	t_fi		*fi;

	//ONE ARGUMENT ONLY: FILENAME
	if (ac != 2)
		fi_error();
	//ELSE ERROR

	//OPEN FILE
	fd = open(av[1], O_RDONLY);
	//IF FILE EXIST, CHECK AND INIT fi STRUCTURE...
	if (fd > 0 && ((fi = fi_parse_file(fd)) != NULL))
		fi_solve_fillit(fi);
	else
		fi_error();
	//ELSE ERROR
	return (0);
}