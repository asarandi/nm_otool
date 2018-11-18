/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asarandi <asarandi@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/18 03:06:51 by asarandi          #+#    #+#             */
/*   Updated: 2018/11/18 03:07:08 by asarandi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "common.h"

int	is_valid_magic(void *map, int *is_64bit, int *is_swapped)
{
	struct mach_header	*mh;

	mh = (struct mach_header *)map;
	if ((mh->magic == MH_MAGIC_64) || (mh->magic == MH_CIGAM_64))
	{
		*is_64bit = 1;
		*is_swapped = (mh->magic == MH_MAGIC_64) ? 0 : 1;
		return (1);
	}
	if ((mh->magic == MH_MAGIC) || (mh->magic == MH_CIGAM))
	{
		*is_64bit = 0;
		*is_swapped = (mh->magic == MH_MAGIC) ? 0 : 1;
		return (1);
	}
	return (0);
}
