#include "ysh.h"

int				ysh_init_env(t_ysh *ysh, char **env)
{
	int			i;
	char		*eq;

	if (env && ysh && ysh->env)
	{
		i = 0;
		while (env[i])
		{
			eq = env[i];
			while (*eq && *eq++ != '=');
			ysh->env[i] = ft_strdup(env[i]);
			// printf("OFFSET= %d\tSTRING= %s\n", (int)(eq - env[i]), eq);
			if (!ft_strncmp(env[i], YSH_PATH, eq - env[i]))
				ysh->paths = ft_strsplit(eq, ':');
				// printf("PATHS= %s\n", env[i]);
			i++;
		}
		return (1);
	}
	return (0);
}

t_ysh			*ysh_init_ysh(int level, int size_env, char **env)
{
	t_ysh		*ysh;

	if ((ysh = (t_ysh *)malloc(sizeof(t_ysh))))
	{
		ysh->ylvl = level;
		ysh->env = (char **)malloc(sizeof(char *) * size_env);
		if (ysh->env && ysh_init_env(ysh, env))
			return (ysh);
	}
	return (NULL);
}
