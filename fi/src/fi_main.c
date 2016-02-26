#include "fi.h"

void		fi_error(void)
{
	ft_putstr("error\n");
	exit(0);
}

int			main(int ac, char **av)
{
	int			fd;
	uintptr_t	fi;

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