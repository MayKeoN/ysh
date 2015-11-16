#include "ysh.h"

void			ysh_print_env(char **env)
{
	if (env)
		while (*env)
			ft_putendl(*env++);
}

void			ysh_print_var(char **var, char sep, char end)
{
	char		*line;

	while ((line = *var++))
	{
		ft_putstr(line);
		(*var) ? ft_putchar(sep) : ft_putchar(end);
	}
}

void			ysh_print_vars(t_ysh *ysh)
{
	ft_putstr("PATHS-> ");
	ysh_print_env(ysh->paths);
	// ysh_print_var(ysh->paths, ':', '\n');
	ft_putstr("YLVL-> ");
	ft_putnbr(ysh->ylvl);
	ft_putstr("\n");
}