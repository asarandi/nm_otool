/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extras.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asarandi <asarandi@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/23 19:15:32 by asarandi          #+#    #+#             */
/*   Updated: 2018/11/23 21:39:42 by asarandi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "common.h"

uint32_t	load_command_segment(t_bin *b)
{
	if (b->is_64bit == 1)
		return (LC_SEGMENT_64);
	else
		return (LC_SEGMENT);
}

/*
** copy n bytes from src to dst, and null terminate dest
*/

char		*ft_strcpx(char *dst, char *src, size_t n)
{
	size_t i;

	i = 0;
	while (i < n)
	{
		if (!src[i])
			break ;
		dst[i] = src[i];
		i++;
	}
	dst[i] = 0;
	return (dst);
}

/*
**int	print_arch_info(t_file *f, void *fat_arch)
**{
**	 uint32_t	cputype;
**   uint32_t	cpusubtype;
**   uint64_t	offset;
**   uint64_t	size;
**   uint32_t	align;
**
**	if (f->is_swapped)
**		cputype = swap32u(((struct fat_arch *)fat_arch)->cputype);
**	else
**		cputype = ((struct fat_arch *)fat_arch)->cputype;
**	if (f->is_swapped)
**		cpusubtype = swap32u(((struct fat_arch *)fat_arch)->cpusubtype);
**	else
**
**	cpusubtype = ((struct fat_arch *)fat_arch)->cpusubtype;
**	if (f->is_fat64)
**	{
**		if (f->is_swapped)
**		{
**			offset = swap64u(((struct fat_arch_64 *)fat_arch)->offset);
**			size   = swap64u(((struct fat_arch_64 *)fat_arch)->size);
**			align  = swap32u(((struct fat_arch_64 *)fat_arch)->align);
**		}
**		else
**		{
**			offset = ((struct fat_arch_64 *)fat_arch)->offset;
**			size   = ((struct fat_arch_64 *)fat_arch)->size;
**			align  = ((struct fat_arch_64 *)fat_arch)->align;
**		}
**	}
**	else
**	{
**
**
**if (f->is_swapped)
**		{
**			offset = swap32u(((struct fat_arch *)fat_arch)->offset);
**			size   = swap32u(((struct fat_arch *)fat_arch)->size);
**			align  = swap32u(((struct fat_arch *)fat_arch)->align);
**		}
**		else
**		{
**			offset = ((struct fat_arch *)fat_arch)->offset;
**			size   = ((struct fat_arch *)fat_arch)->size;
**			align  = ((struct fat_arch *)fat_arch)->align;
**		}
**	}
**	ft_printf("ARCH: cputype = %08x, cpusubtype = %08x, \
**		offset = %016llx, size = %016llx, align = %08x\n",
**			cputype, cpusubtype, offset, size, align);
**	return (0);
**}
**
**
**void fat_print_arches(t_file *f)
**{
**	uint32_t	i;
**	void		*fat_arch;
**
**	ft_printf("ARCH: %s\n", f->fn);
**	i = 0;
**	while (i < file_nfat_arch(f))
**	{
**		fat_arch = &f->map[
**			sizeof(struct fat_header) + (sizeof_fat_arch(f) * i)];
**		(void)print_arch_info(f, fat_arch);
**
**		i++;
**	}
**	return ;
**}
*/
