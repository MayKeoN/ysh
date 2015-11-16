#ifndef YSH_DEF_H
# define YSH_DEF_H

# include "libft.h"
# include <stdio.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <dirent.h>


// # define YSH_PATHS 		"/usr/local/bin:/usr/bin:/bin"
// # define YSH_EXIT 		"exit"
# define YSH_PATH 		"PATH="
// # define YSH_USER 		"USER="
// # define YSH_PWD 		"PWD="
// # define YSH_OLDPWD 	"OLDPWD="

typedef struct	s_ysh
{
	char		**env;
	int			ylvl;
	char		**paths;
}				t_ysh;

#endif