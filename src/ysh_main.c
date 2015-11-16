#include "ysh.h"

extern char		**environ;

int				main(void)
{
	t_ysh		*ysh;
	size_t		size_env;

	size_env = ysh_tablen(environ);
	ysh = ysh_init_ysh(1, size_env, environ);
	// ysh_print_env(ysh->env);
	// ysh_print_vars(ysh);
	ysh_prompt(ysh);
	return (0);
}