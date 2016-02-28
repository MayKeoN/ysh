#ifndef FI_DEF_H
# define FI_DEF_H

# include <fcntl.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdint.h>

# include <stdio.h>
# include "libft.h"

# define BIT_R (unsigned short int)0x0001
# define BIT_L (unsigned short int)0x8000
# define BIT_FULL (unsigned short int)0xFFFF

# define NIB_000F (unsigned short int)0x000F
# define NIB_00F0 (unsigned short int)0x00F0
# define NIB_0F00 (unsigned short int)0x0F00
# define NIB_F000 (unsigned short int)0xF000

typedef struct 				s_fi
{
	char					*file;
	int						fd;
	unsigned short int		tios[26];
	int						ntios;
	int						msize;
	int						size;
	int						max;
	unsigned short int		*m;
	int						*idc;
}							t_fi;

#endif