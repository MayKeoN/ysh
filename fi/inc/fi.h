#ifndef FI_H
# define FI_H

# include "fi_def.h"

int						fi_init_sizeit(int n);
unsigned short int		*fi_init_smap(int min);
void					fi_display_map(t_fi *fi);
void					fi_display_smap(t_fi *fi);

t_fi					*fi_parse_file(int fd);
void					fi_solve_curse(int index, int k, int *done, t_fi *fi);



#endif