#ifndef FI_H
# define FI_H

# include "fi_def.h"

t_fi					*fi_parse_file(int fd);

void					fi_solve_fillit(t_fi *fi);

void					fi_display_short(unsigned short int n);
void					fi_display_short_inline(unsigned short int n);
void					fi_display_smap(t_fi *fi);

unsigned long int		fi_stl(unsigned short int n);

int						fi_init_sizeit(int n);
unsigned short int		*fi_init_smap(int min);



#endif