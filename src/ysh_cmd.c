#include "ysh.h"

void			ysh_exec_cmd(char **env, char *cmd_path, char **cmd_args)
{
	if (cmd_path)
	{

		if (!fork())
		{
			execve(cmd_path, cmd_args, env);
			exit(0);
		}
		else
			wait(NULL);
	}
}

int				ysh_cmd_nn(t_ysh *ysh, char **paths, char *cmd, char **cmd_args)
{
	char		*path;
	char		*cmd_path;
	DIR			*dir;

	while (*paths)
	{
		path = ft_strjoin(*paths, "/");
		if ((dir = opendir(path)))
		{
			cmd_path = ft_strjoin(path, cmd);
			if (cmd_path)
			{
				// printf("CMDPATH= %s\n", cmd_path);
				if (!access(cmd_path, X_OK))
				{
					ysh_exec_cmd(ysh->env, cmd_path, cmd_args);
					free(cmd_path);
					return (1);
				}	
				free(cmd_path);
			}
		}
		paths++;
	}
	return (0);
}


void			ysh_cmd_parse(t_ysh *ysh, char *cmd)
{
	char		**cmd_args;

	cmd_args = ft_strsplit(cmd, ' ');
	if (!ft_strncmp(cmd, "exit", 5))
		exit(0);
	else if (!ft_strncmp(cmd, "env", 4))
		ysh_print_env(ysh->env);
	else if (!ft_strncmp(cmd, "vars", 4))
		ysh_print_vars(ysh);
	else if (!ft_strncmp(cmd, "unsetenv", 9))
		exit(0);
	else if (!ft_strncmp(cmd, "setenv", 7))
		exit(0);
	else
	{
		ysh_cmd_nn(ysh, ysh->paths, cmd_args[0], cmd_args);

		// printf("YOUHOU\n");
	}
	// ysh_print_vars(ysh);
	free(cmd_args);
}