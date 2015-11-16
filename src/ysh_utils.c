#include "ysh.h"

size_t			ysh_tablen(char **tab)
{
	char		*start;

	start = NULL;
	if (tab)
	{
		start = *tab;
		while (*tab++);
	}
	return ((size_t)(*tab - start));
}