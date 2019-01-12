/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnicmp.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asarandi <asarandi@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/20 15:40:36 by asarandi          #+#    #+#             */
/*   Updated: 2019/01/12 01:14:00 by asarandi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strnicmp(const char *s1, const char *s2, size_t n)
{
	size_t			i;
	unsigned char	a;
	unsigned char	b;

	i = 0;
	while ((i < n) && (s1[i]) && (s2[i]))
	{
		a = (unsigned char)s1[i];
		b = (unsigned char)s2[i];
		if (ft_isalpha(a) && ft_isalpha(b))
		{
			if (a <= 'Z')
				a += 'a' - 'A';
			if (b <= 'Z')
				b += 'a' - 'A';
		}
		if (a != b)
			return (a - b);
		i++;
	}
	if (i == n)
		return (0);
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}
