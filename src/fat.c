/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fat.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asarandi <asarandi@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/23 19:20:49 by asarandi          #+#    #+#             */
/*   Updated: 2018/11/23 19:21:11 by asarandi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "common.h"

uint32_t	file_nfat_arch(t_file *f)
{
	if (f->is_swapped)
		return (swap32u(((struct fat_header *)f->map)->nfat_arch));
	else
		return (((struct fat_header *)f->map)->nfat_arch);
}

uint64_t	fat_arch_offset(t_file *f, void *header)
{
	if (f->is_fat64)
	{
		if (f->is_swapped)
			return (swap64u(((struct fat_arch_64 *)header)->offset));
		else
			return (((struct fat_arch_64 *)header)->offset);
	}
	else
	{
		if (f->is_swapped)
			return (swap32u(((struct fat_arch *)header)->offset));
		else
			return (((struct fat_arch *)header)->offset);
	}
}

uint64_t	fat_arch_size(t_file *f, void *header)
{
	if (f->is_fat64)
	{
		if (f->is_swapped)
			return (swap64u(((struct fat_arch_64 *)header)->size));
		else
			return (((struct fat_arch_64 *)header)->size);
	}
	else
	{
		if (f->is_swapped)
			return (swap32u(((struct fat_arch *)header)->size));
		else
			return (((struct fat_arch *)header)->size);
	}
}

uint32_t	sizeof_fat_arch(t_file *f)
{
	if (f->is_fat64)
		return (sizeof(struct fat_arch_64));
	else
		return (sizeof(struct fat_arch));
}
