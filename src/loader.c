/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loader.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asarandi <asarandi@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/23 19:39:16 by asarandi          #+#    #+#             */
/*   Updated: 2018/11/23 22:48:40 by asarandi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "common.h"

/*
** validate the magic and check load commands sizes;
** returns 0 on success, 1 on failure (prints error msg)
** will populate (t_bin *) with values for other functions
*/

int	is_valid_magic(void *single, int *is_64bit, int *is_swapped)
{
	struct mach_header	*mh;

	mh = (struct mach_header *)single;
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

int	validate_macho(t_bin *b)
{
	uint32_t			i;
	off_t				offset;
	struct load_command	*lc;

	if (!is_valid_magic(b->data, &b->is_64bit, &b->is_swapped))
		return (msgerr(E_BADMAGIC_ERR, b->fn));
	b->mh = (struct mach_header *)b->data;
	b->ncmds = swap32(b, b->mh->ncmds);
	offset = sizeof_mach_header(b);
	i = 0;
	while ((i < b->ncmds) && (offset < b->fsize))
	{
		lc = (struct load_command *)&b->data[offset];
		offset += swap32(b, lc->cmdsize);
		i++;
	}
	if (offset > b->fsize)
		return (msgerr(E_BADOFFSET_ERR, b->fn));
	return (0);
}

int	process_macho(t_bin *b, void (*func)(t_bin *))
{
	if (validate_macho(b) != 0)
		return (1);
	(void)func(b);
	return (0);
}

int	binary_loader(t_file *f, void (*func)(t_bin *))
{
	t_bin		b;

	if (is_macho_file(f))
	{
		(void)ft_memset(&b, 0, sizeof(t_bin));
		b.data = f->map;
		b.fn = f->fn;
		b.fsize = f->st.st_size;
		b.parent = f;
		return (process_macho(&b, func));
	}
	else if (is_fat_file(f))
		return (fat_file_loader(f, func));
	else if (is_archive_file(f))
		return (archive_file_loader(f, func));
	else
		return (msgerr(E_BADFTYPE_ERR, f->fn));
}

int	process_file(t_file *f, void (*func)(t_bin *))
{
	if ((f->fd = open(f->fn, O_RDONLY)) == -1)
		return (msgerr(E_OPEN_ERR, f->fn));
	if (fstat(f->fd, &f->st) == -1)
		return (fclose_msgerr(f->fd, E_FSTAT_ERR, f->fn));
	if (f->st.st_size < 1)
		return (fclose_msgerr(f->fd, E_FILE_EMPTY, f->fn));
	if (!(f->st.st_mode & S_IFREG))
		return (fclose_msgerr(f->fd, E_FNOTREG_ERR, f->fn));
	f->map = mmap(0, f->st.st_size, PROT_READ, MAP_SHARED, f->fd, 0);
	if (f->map == MAP_FAILED)
		return (fclose_msgerr(f->fd, E_MMAP_ERR, f->fn));
	(void)binary_loader(f, func);
	if (munmap(f->map, f->st.st_size) == -1)
		return (fclose_msgerr(f->fd, E_MUNMAP_ERR, f->fn));
	(void)close(f->fd);
	return (0);
}
