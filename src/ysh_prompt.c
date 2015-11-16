#include "ysh.h"

void			ysh_prompt(t_ysh *ysh)
{
	char		*cmd;
	
	while (42)
	{
		ft_putstr("$ysh> ");
		get_next_line(0, &cmd);
		if (*cmd)
		{
			// printf("CMD= %s\n", cmd);
			ysh_cmd_parse(ysh, cmd);
		}
	}
}