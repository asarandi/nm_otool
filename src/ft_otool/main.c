/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asarandi <asarandi@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/16 01:02:57 by asarandi          #+#    #+#             */
/*   Updated: 2018/11/21 20:44:18 by asarandi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "otool.h"


#define OTOOL_ROW_LENGTH 16

uint64_t	section_addr(t_bin *b, void *sect)
{
	if (b->is_64bit == 1)
		return (swap64(b, ((struct section_64 *) sect)->addr));
	else
		return (swap32(b, ((struct section *) sect)->addr));
}

uint64_t	section_size(t_bin *b, void *sect)
{
	if (b->is_64bit == 1)
		return (swap64(b, ((struct section_64 *) sect)->size));
	else
		return (swap32(b, ((struct section *) sect)->size));
}

uint32_t	section_offset(t_bin *b, void *sect)
{
	if (b->is_64bit == 1)
		return (swap32(b, ((struct section_64 *) sect)->offset));
	else
		return (swap32(b, ((struct section *) sect)->offset));
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

int	ft_otool_print_section(t_bin *b, void *seg, void *sect)
{
	uint64_t		i;
	uint64_t		size;
	uint64_t		addr;
	uint32_t		offset;
	unsigned char	c;

	(void)ft_printf("%s:\nContents of (%s,%s) section", b->fn, 
			((struct segment_command *) seg)->segname,
			((struct section *) sect)->sectname);
	i = 0;
	offset = section_offset(b, sect);
	size = section_size(b, sect);
	addr = section_addr(b, sect);
	while (i < size)
	{
		if (i % OTOOL_ROW_LENGTH == 0)
		{
			if (b->is_64bit == 1)
				ft_printf("\n%016lx\t", addr + i);
			else
				ft_printf("\n%08x\t", addr + i);
		}
		c = ((unsigned char *)b->data)[offset + i];
		(void)otool_print_byte(c);
		i++;
	}
	(void)ft_printf("\n");
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

int	otool_find(t_bin *b, char *s1, char *s2)
{
	void		*seg;
	void		*sec;
	uint32_t	i;
	uint32_t	j;

	if (validate_macho(b) != 0)
		return (1);
	i = 0;
	while ((seg = get_segment_by_name_idx(b, s1, i++)) != NULL)
	{
		j = 0;
		while ((sec = get_section_by_name_idx(b, seg, s2, j++)) != NULL)
		{
			(void)ft_otool_print_section(b, seg, sec);
		}
	}
	return (0);
}


int	process_file(t_file *f)
{
	t_bin	b;

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
//-----------------------------------------
	(void)ft_memset(&b, 0, sizeof(t_bin));
	b.data = f->map;	/* XXX */
	b.fsize = f->st.st_size;
	b.fn = f->fn;
	(void)otool_find(&b, "__TEXT", "__text");
//-------------------------------------------	
	if (munmap(f->map, f->st.st_size) == -1)
		return (fclose_msgerr(f->fd, E_MUNMAP_ERR, f->fn));
	(void)close(f->fd);
	return (0);
}

int main(int ac, char **av)
{
	int			i;
	t_file		f;

	i = 1;
	while (i < ac)
	{
		(void)ft_memset(&f, 0, sizeof(t_file));
		f.fn = av[i];
		(void)process_file(&f);
		i++;
	}
	if (i == 1)
		(void)show_usage(av[0]);
	return (0);
}
