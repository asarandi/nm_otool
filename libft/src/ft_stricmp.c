/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_stricmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asarandi <asarandi@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/20 15:40:36 by asarandi          #+#    #+#             */
/*   Updated: 2018/05/26 20:38:57 by asarandi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_stricmp(const char *s1, const char *s2)
{
	size_t			i;
	unsigned char	a;
	unsigned char	b;

	i = 0;
	while ((s1[i]) && (s2[i]))
	{
		a = (unsigned char)s1[i];
		b = (unsigned char)s2[i];
		if ((((a >= 'a') && (a <= 'z')) || ((a >= 'A') && (a <= 'Z'))) &&
				(((b >= 'a') && (b <= 'z')) || ((b >= 'A') && (b <= 'Z'))))
		{
			if (a <= 'Z')
				a += 'a' - 'A';
			if (b <= 'Z')
				b += 'a' - 'A';
		}
		if (a != b)
			return ((unsigned char)(a - b));
		else
			i++;
	}
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}
