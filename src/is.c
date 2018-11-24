/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asarandi <asarandi@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/23 19:22:53 by asarandi          #+#    #+#             */
/*   Updated: 2018/11/23 19:25:50 by asarandi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "common.h"

int	is_macho_file(t_file *f)
{
	uint32_t	magic;

	magic = ((struct mach_header *)f->map)->magic;
	if ((magic == MH_MAGIC) || (magic == MH_CIGAM))
		return (1);
	else if ((magic == MH_MAGIC_64) || (magic == MH_CIGAM_64))
		return (1);
	return (0);
}

int	is_fat_file(t_file *f)
{
	uint32_t	magic;

	magic = ((struct fat_header *)f->map)->magic;
	f->is_fat = 0;
	if ((magic == FAT_MAGIC) || (magic == FAT_CIGAM))
	{
		f->is_fat = 1;
		if (magic == FAT_CIGAM)
			f->is_swapped = 1;
	}
	else if ((magic == FAT_MAGIC_64) || (magic == FAT_CIGAM_64))
	{
		f->is_fat = 1;
		f->is_fat64 = 1;
		if (magic == FAT_CIGAM_64)
			f->is_swapped = 1;
	}
	return (f->is_fat);
}

int	is_archive_file(t_file *f)
{
	f->is_archive = 0;
	if (ft_strncmp((char *)f->map, ARMAG, SARMAG) == 0)
		f->is_archive = 1;
	return (f->is_archive);
}

int	is_symdef(char *s)
{
	if (ft_strcmp(s, SYMDEF) == 0)
		return (1);
	if (ft_strcmp(s, SYMDEF_SORTED) == 0)
		return (1);
	if (ft_strcmp(s, SYMDEF_64) == 0)
		return (1);
	if (ft_strcmp(s, SYMDEF_64_SORTED) == 0)
		return (1);
	return (0);
}

int		is_print_symaddr(char c)
{
	if ((c == 'U') || (c == 'u'))
		return (0);
	if ((c == 'I') || (c == 'i'))
		return (0);
	return (1);
}
