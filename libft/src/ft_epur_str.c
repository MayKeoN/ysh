/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_epur_str.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cglavieu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/11/16 00:40:03 by cglavieu          #+#    #+#             */
/*   Updated: 2015/11/16 00:40:06 by cglavieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int			things(const char *src)
{
	int		hint;
	int		a;

	a = -1;
	hint = 0;
	while (*(src + ++a) != '\0')
	{
		if (*(src + a) == ' ' || *(src + a) == '\t')
			hint++;
	}
	return (hint);
}

char		*ft_epur_str(const char *src)
{
	int		count;
	int		a;
	char	*ret;

	a = 0;
	count = -1;
	ret = (char *)malloc(sizeof(char) * (ft_strlen(src) - things(src) + 1));
	ft_bzero(ret, ft_strlen(src) - things(src) + 1);
	while (*(src + ++count + a) != '\0')
	{
		while (*(src + count + a) == ' ' || *(src + count + a) == '\t')
		{
			a++;
		}
		*(ret + count) = *(src + count + a);
	}
	return (ret);
}
