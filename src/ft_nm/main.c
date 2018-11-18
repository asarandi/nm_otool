/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asarandi <asarandi@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/16 01:02:15 by asarandi          #+#    #+#             */
/*   Updated: 2018/11/18 09:24:37 by asarandi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"
#define debug	1

/*
** returns matching load command by index or NULL
** for example 2nd LC_SEGMENT or first LC_SYMTAB
*/

t_lc	*get_lcmd_by_index(t_machof *f, uint32_t cmd, uint32_t idx)
{
	uint32_t	i;
	uint32_t	counter;
	off_t		offset;
	t_lc		*lcmd;

	i = 0;
	counter = 0;
	offset = sizeof(struct mach_header_64);
	if (f->is_64bit == 0)
		offset = sizeof(struct mach_header);
	while (i < f->ncmds)
	{
		lcmd = (struct load_command *)&f->map[offset];
		if (swap32(f->is_swapped, lcmd->cmd) == cmd)
		{
			if (counter == idx)
				return (lcmd);
			counter++;
		}
		offset += swap32(f->is_swapped, lcmd->cmdsize);
		i++;
	}
	return (NULL);
}

/*
** search macho for a segment with a specific name
** index is used in case there's multiple occurances of same seg name
*/

void	*get_segment_by_name_idx(t_machof *f, char *segname, uint32_t idx)
{
	uint32_t	lctype;
	void		*segment;
	uint32_t	iseg;
	uint32_t	matches;
	
	lctype = LC_SEGMENT_64;
	if (f->is_64bit == 0)
		lctype = LC_SEGMENT;
	iseg = 0;
	matches = 0;
	while ((segment = get_lcmd_by_index(f, lctype, iseg++)) != NULL)
	{
		if ((ft_strcmp(((struct segment_command *) segment)->segname,
					segname)) == 0)
		{
			if (matches == idx)
				return (segment);
			matches++;
		}
	}
	return (NULL);
}


uint32_t	sizeof_nlist(t_machof *f)
{
	if (f->is_64bit == 1)
		return (sizeof(struct nlist_64));
	else
		return (sizeof(struct nlist));
}

uint32_t	sizeof_segment(t_machof *f)
{
	if (f->is_64bit == 1)
		return (sizeof(struct segment_command_64));
	else
		return (sizeof(struct segment_command));

}

uint32_t	sizeof_section(t_machof *f)
{
	if (f->is_64bit == 1)
		return (sizeof(struct section_64));
	else
		return (sizeof(struct section));
}

uint32_t	nsects_in_segment(t_machof *f, void *segment)
{
	if (f->is_64bit == 1)
		return (((struct segment_command_64 *) segment)->nsects);
	else
		return (((struct segment_command *) segment)->nsects);
}

uint32_t	sizeof_mach_header(t_machof *f)
{
	if (f->is_64bit == 1)
		return (sizeof(struct mach_header_64));
	else
		return (sizeof(struct mach_header));
}

void	*get_section_by_name_idx(t_machof *f, void *seg, char *sn, uint32_t i)
{
	uint32_t	matches;
	uint32_t	nsects;
	uint32_t	k;

	nsects = nsects_in_segment(f, seg);
	seg += sizeof_segment(f);
	k = 0;
	matches = 0;
	while (k < nsects)
	{
		if (ft_strcmp(((struct section *) seg)->sectname, sn) == 0)
		{
			if (matches == i)
				return (seg);
			matches++;
		}
		seg += sizeof_section(f);
		k++;
	}
	return (NULL);
}




//struct section
//struct section_64

void	*get_section_by_number(t_machof *f, uint8_t sect)
{
	uint32_t	lctype;
	void		*segment;
	uint32_t	iseg;
	uint32_t	nsects;
	uint32_t	total_sects;
	

	lctype = LC_SEGMENT_64;
	if (f->is_64bit == 0)
		lctype = LC_SEGMENT;
	iseg = 0;
	total_sects = 0;
	while ((segment = get_lcmd_by_index(f, lctype, iseg++)) != NULL)
	{
		nsects = nsects_in_segment(f, segment);
		if (nsects > 0)
		{
			if ((sect >= total_sects) && (sect <= total_sects + nsects))
				return (segment);
		}
		total_sects += nsects;
	}

	return (NULL);
}

//
//struct nlist {
//	union {
//#ifndef __LP64__
//		char *n_name;	/* for use when in-core */
//#endif
//		uint32_t n_strx;	/* index into the string table */
//	} n_un;
//	uint8_t n_type;		/* type flag, see below */
//	uint8_t n_sect;		/* section number or NO_SECT */
//	int16_t n_desc;		/* see <mach-o/stab.h> */
//	uint32_t n_value;	/* value of this symbol (or stab offset) */
//};

/*
 * This is the symbol table entry structure for 64-bit architectures.
 */
//struct nlist_64 {
//    union {
//        uint32_t  n_strx; /* index into the string table */
//    } n_un;
//    uint8_t n_type;        /* type flag, see below */
//    uint8_t n_sect;        /* section number or NO_SECT */
//    uint16_t n_desc;       /* see <mach-o/stab.h> */
//    uint64_t n_value;      /* value of this symbol (or stab offset) */
//};
//



//---------------------------------------------------------------------------

/*


   U (undefined)
   A (absolute)
   T (text section symbol)
   D (data section symbol)
   B (bss section symbol)
   C (common symbol)
   - (for debugger symbol table entries)
   S (symbol in a section other than those above)
   I (indirect symbol)
   -------------------
   lowercase letters if the symbol is local


//---------------------------------------------------------------------------

       Each symbol name is preceded by its value (blanks if undefined).  Unless the -m option is specified, this value is followed by one of the following characters, representing the symbol type: U (undefined), A (absolute), T (text section symbol), D (data section symbol), B (bss section symbol),  C  (common  sym-
       bol),  -  (for  debugger  symbol table entries; see -a below), S (symbol in a section other than those above), or I (indirect symbol).  If the symbol is local (non-external), the symbol's type is instead represented by the corresponding lowercase letter.  A lower case u in a dynamic shared library indicates a
       undefined reference to a private external in another module in the same library.

       If the symbol is a Objective C method, the symbol name is +-[Class_name(category_name) method:name:], where `+' is for class methods, `-' is for instance methods, and (category_name) is present only when the method is in a category.

       The output is sorted alphabetically by default.
*/


void	debug1(t_machof *f)
{
	ft_printf("LC_SYMTAB COMMAND at %08x\n", (void *)f->stc - f->map);
	ft_printf("    cmd = %08x\n", swap32(f->is_swapped, f->stc->cmd));
	ft_printf("cmdsize = %08x\n", swap32(f->is_swapped, f->stc->cmdsize));
	ft_printf(" symoff = %08x\n", swap32(f->is_swapped, f->stc->symoff));
	ft_printf("  nsyms = %08x\n", swap32(f->is_swapped, f->stc->nsyms));
	ft_printf(" stroff = %08x\n", swap32(f->is_swapped, f->stc->stroff));
	ft_printf("strsize = %08x\n", swap32(f->is_swapped, f->stc->strsize));
}

int	nm_symtab(t_machof *f)
{
	if (debug)	debug1(f);

	uint32_t		i;
	uint32_t		symoff;
	uint32_t		nsyms;
	uint32_t		stroff;
	uint32_t		strsize;
	ssize_t			symsize;
	struct nlist	*nlist;
	uint32_t		n_strx;
	char			*symname;

	symoff = swap32(f->is_swapped, f->stc->symoff);
	nsyms = swap32(f->is_swapped, f->stc->nsyms);
	stroff = swap32(f->is_swapped, f->stc->stroff);
	strsize = swap32(f->is_swapped, f->stc->strsize);
	symsize = sizeof_nlist(f);
	i = 0;

	while (i < nsyms)
	{
		nlist = (struct nlist *)&f->map[symoff + i * symsize];
		n_strx = swap32(f->is_swapped, nlist->n_un.n_strx);
		symname = (char *)&f->map[stroff + n_strx];

		if ((n_strx != 0) && (*symname != 0))
		{

		uint8_t type = nlist->n_type;
		uint8_t sect = nlist->n_sect;
		uint16_t desc = swap16(f->is_swapped, nlist->n_desc);
		uint64_t value;
		void	*section;

		if (f->is_64bit)
			value = swap64(f->is_swapped, nlist->n_value);
		else
			value = swap32(f->is_swapped, nlist->n_value);

		if (sect != 0)
		{
			section = get_section_by_number(f, sect);

			uint64_t valueX;

			if (f->is_64bit)
				valueX = swap64(f->is_swapped, ((struct segment_command_64 *)section)->vmaddr );
			else
				valueX = swap32(f->is_swapped, ((struct segment_command *) section)->vmaddr);
			ft_printf("valueX = %016llx, ", valueX);

		}



		ft_printf("index = %08x, n_strx = %08x, type = %02hhx, sect = %02hhx, desc = %04hx, ", i, n_strx, type, sect, desc);
		if (f->is_64bit)
			ft_printf("value = %016llx, name = %s\n", value, symname);
		else
			ft_printf("value = %08x, name = %s\n", value, symname);
		}

		i++;
	}


	return (0);
}



/*
** validate the magic and check load commands sizes;
** returns 0 on success, 1 on failure (prints error msg)
** will populate t_machof with values for other functions
*/


int	validate_macho(t_machof *f)
{
	uint32_t			i;
	off_t				offset;
	struct load_command	*lc;


	if (!is_valid_magic(f->map, &f->is_64bit, &f->is_swapped))
		return (msgerr(E_BADMAGIC_ERR, f->fn));
	f->mh = (struct mach_header *)f->map;
	f->ncmds = swap32(f->is_swapped, f->mh->ncmds);
	offset = sizeof_mach_header(f);
	i = 0;
	while ((i < f->ncmds) && (offset < f->st.st_size))
	{
		lc = (struct load_command *)&f->map[offset];
		offset += swap32(f->is_swapped, lc->cmdsize);
		i++;
	}
	if (offset > f->st.st_size)
		return (msgerr(E_BADOFFSET_ERR, f->fn));
	return (0);
}


int	nm(t_machof *f)
{
	uint32_t				isym;
	struct symtab_command	*stc;
	struct load_command		*lcmd;

	if (validate_macho(f) != 0)
		return (1);
	isym = 0;
	while ((lcmd = get_lcmd_by_index(f, LC_SYMTAB, isym++)) != NULL)
	{
		stc = (struct symtab_command *)lcmd;
		f->stc = stc;
		(void)nm_symtab(f);

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
	(void)nm(f);
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
