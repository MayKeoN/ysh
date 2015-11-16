#ifndef YSH_H
# define YSH_H

# include "ysh_def.h"

void			ysh_print_env(char **env);
void			ysh_print_vars(t_ysh *ysh);

void			ysh_prompt(t_ysh *ysh);

void			ysh_cmd_parse(t_ysh *ysh, char *cmd);



int				ysh_init_env(t_ysh *ysh, char **env);
t_ysh			*ysh_init_ysh(int level, int size_env, char **env);

size_t			ysh_tablen(char **tab);

#endif