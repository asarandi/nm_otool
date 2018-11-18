/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asarandi <asarandi@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/16 01:02:57 by asarandi          #+#    #+#             */
/*   Updated: 2018/11/18 11:44:13 by asarandi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "otool.h"
#define OTOOL_ROW_LENGTH 16

uint64_t	section_addr(t_machof *f, void *sect)
{
	if (f->is_64bit == 1)
		return (swap64(f, ((struct section_64 *) sect)->addr));
	else
		return (swap32(f, ((struct section *) sect)->addr));
}

uint64_t	section_size(t_machof *f, void *sect)
{
	if (f->is_64bit == 1)
		return (swap64(f, ((struct section_64 *) sect)->size));
	else
		return (swap32(f, ((struct section *) sect)->size));
}

uint32_t	section_offset(t_machof *f, void *sect)
{
	if (f->is_64bit == 1)
		return (swap32(f, ((struct section_64 *) sect)->offset));
	else
		return (swap32(f, ((struct section *) sect)->offset));
}

int	otool_print_byte(unsigned char c)
{
	const char hex[] = "0123456789abcdef";
	char out[3];

	out[0] = hex[c / 16];
	out[1] = hex[c % 16];
	out[2] = ' ';
	write(1, &out[0], 3);
	return (0);
}

int	ft_otool_print_section(t_machof *f, void *seg, void *sect)
{
	uint64_t		i;
	uint64_t		size;
	uint64_t		addr;
	uint32_t		offset;
	unsigned char	b;

	(void)ft_printf("%s:\nContents of (%s,%s) section", f->fn, 
			((struct segment_command *) seg)->segname,
			((struct section *) sect)->sectname);
	i = 0;
	offset = section_offset(f, sect);
	size = section_size(f, sect);
	addr = section_addr(f, sect);
	while (i < size)
	{
		if (i % OTOOL_ROW_LENGTH == 0)
		{
			if (f->is_64bit == 1)
				ft_printf("\n%016lx\t", addr + i);
			else
				ft_printf("\n%08x\t", addr + i);
		}
		b = ((unsigned char *)f->map)[offset+i];
		(void)otool_print_byte(b);
//		ft_printf("%02hhx ", b);
		i++;
	}
	(void)ft_printf("\n");
	return (0);
}

int	validate_macho(t_machof *f)
{
	uint32_t			i;
	off_t				offset;
	struct load_command	*lc;


	if (!is_valid_magic(f->map, &f->is_64bit, &f->is_swapped))
		return (msgerr(E_BADMAGIC_ERR, f->fn));
	f->mh = (struct mach_header *)f->map;
	f->ncmds = swap32(f, f->mh->ncmds);
	offset = sizeof_mach_header(f);
	i = 0;
	while ((i < f->ncmds) && (offset < f->st.st_size))
	{
		lc = (struct load_command *)&f->map[offset];
		offset += swap32(f, lc->cmdsize);
		i++;
	}
	if (offset > f->st.st_size)
		return (msgerr(E_BADOFFSET_ERR, f->fn));
	return (0);
}

int	otool_find(t_machof *f, char *s1, char *s2)
{
	void		*seg;
	void		*sec;
	uint32_t	i;
	uint32_t	j;

	if (validate_macho(f) != 0)
		return (1);
	i = 0;
	while ((seg = get_segment_by_name_idx(f, s1, i++)) != NULL)
	{
		j = 0;
		while ((sec = get_section_by_name_idx(f, seg, s2, j++)) != NULL)
		{
			(void)ft_otool_print_section(f, seg, sec);
		}
	}
	return (0);
}


int	process_file(t_machof *f)
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
	(void)otool_find(f, "__TEXT", "__text");
	if (munmap(f->map, f->st.st_size) == -1)
		return (fclose_msgerr(f->fd, E_MUNMAP_ERR, f->fn));
	(void)close(f->fd);
	return (0);
}

int main(int ac, char **av)
{
	int			i;
	t_machof	f;

	i = 1;
	while (i < ac)
	{
		(void)ft_memset(&f, 0, sizeof(t_machof));
		f.fn = av[i];
		(void)process_file(&f);
		i++;
	}
	if (i == 1)
		(void)show_usage(av[0]);
	return (0);
}
