/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asarandi <asarandi@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/16 01:02:15 by asarandi          #+#    #+#             */
/*   Updated: 2018/11/19 11:23:42 by asarandi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"
#define debug	0


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
	ft_printf("    cmd = %08x\n", swap32(f, f->stc->cmd));
	ft_printf("cmdsize = %08x\n", swap32(f, f->stc->cmdsize));
	ft_printf(" symoff = %08x\n", swap32(f, f->stc->symoff));
	ft_printf("  nsyms = %08x\n", swap32(f, f->stc->nsyms));
	ft_printf(" stroff = %08x\n", swap32(f, f->stc->stroff));
	ft_printf("strsize = %08x\n", swap32(f, f->stc->strsize));
}

/*
** logic: the nlist structure has the 'n_sect' member,
** which is section number where symbol lives
**
*/




char	get_character_for_symbol(t_machof *f, uint8_t n_sect)
{
	void	*sect;
	char	*sectname;
	char	*segname;

	sect = get_section_by_number(f, n_sect);
	if (sect != NULL)
	{
		sectname = ((struct section *) sect)->sectname;
		segname = ((struct section *) sect)->segname;
		if ((ft_strcmp(segname, SEG_TEXT) == 0) &&		//SEG_TEXT
				(ft_strcmp(sectname, SECT_TEXT) == 0))	//SECT_TEXT
			return ('T');
		if ((ft_strcmp(segname, SEG_DATA) == 0) &&		//SEG_DATA
				(ft_strcmp(sectname, SECT_DATA) == 0))	//SECT_DATA
			return ('D');
		if ((ft_strcmp(segname, SEG_DATA) == 0) &&		//SEG_DATA
				(ft_strcmp(sectname, SECT_BSS) == 0))	//SECT_BSS
			return ('B');
//		if ((ft_strcmp(segname, SEG_DATA) == 0) &&		//SEG_DATA
//				(ft_strcmp(sectname, SECT_COMMON) == 0))//SECT_COMMON
//			return ('C');
	}
	return ('S');
}


/*
typedef struct s_stc
{
	t_machof				*f;
	struct symtab_command	*stc;
	uint32_t				symoff;
	uint32_t				nsyms;
	struct nlist			*nlist;
}				t_stc;


void		populate_stc_struct(t_machof *f, struct symtab_command *stc)
{

}
*/


typedef struct symtab_command	t_stc;
typedef struct nlist			t_nlist;


/*
** copied count_symbols_to_display()
*/

t_nlist	**populate_symptr_array(t_machof *f, t_stc *stc, t_nlist **array)
{
	uint32_t		i;
	uint32_t		j;
	uint32_t		symoff;
	struct nlist	*nlist;

	symoff = swap32(f, stc->symoff);
	i = 0;
	j = 0;
	while (i < swap32(f, stc->nsyms))
	{
		nlist = (struct nlist *)&f->map[symoff + i * sizeof_nlist(f)];
		if (!(nlist->n_type & N_STAB))
		{
			array[j++] = nlist;
		}
		i++;
	}
	array[j] = NULL;
	return (array);
}

uint32_t	count_symbols_to_display(t_machof *f, t_stc *stc)
{
	uint32_t		i;
	uint32_t		count;
	uint32_t		symoff;
	struct nlist	*nlist;

	symoff = swap32(f, stc->symoff);
	i = 0;
	count = 0;
	while (i < swap32(f, stc->nsyms))
	{
		nlist = (struct nlist *)&f->map[symoff + i * sizeof_nlist(f)];
		if (!(nlist->n_type & N_STAB))
			count++;
		i++;
	}
	return (count);
}


int		compare_symbol_names(t_machof *f, t_stc *stc, t_nlist *a, t_nlist *b)
{
	uint32_t	stroff;
	uint32_t	n_strx;
	char		*a_name;
	char		*b_name;

	stroff = swap32(f, stc->stroff);
	n_strx = swap32(f, a->n_un.n_strx);
	a_name = (char *)&f->map[stroff + n_strx];
	n_strx = swap32(f, b->n_un.n_strx);
	b_name = (char *)&f->map[stroff + n_strx];
	return (ft_strcmp(a_name, b_name));
}

/*
** https://en.wikipedia.org/wiki/Bubble_sort
*/

t_nlist	**sort_symptr_array(t_machof *f, t_stc *stc, t_nlist **a, uint32_t n)
{
	uint32_t	i;
	int			swapped;
	t_nlist		*tmp;

	while (1)
	{
		swapped = 0;
		i = 1;
		while (i <= n - 1)
		{
			if (compare_symbol_names(f, stc, a[i - 1], a[i]) > 0)
			{
				tmp = a[i - 1];
				a[i - 1] = a[i];
				a[i] = tmp;
				swapped = 1;
			}
			i++;
		}
		if (!swapped)
			break ;
	}
	return (a);
}



uint64_t	nlist_n_value(t_machof *f, t_nlist *nlist)
{
	uint64_t	value;

	if (f->is_64bit)
		value = swap64(f, nlist->n_value);
	else
		value = swap32(f, nlist->n_value);
	return (value);
}

uint64_t	segment_vmaddr(t_machof *f, void *seg)
{
	uint64_t	res;

	if (f->is_64bit)
		res = swap64(f, ((struct segment_command_64 *)seg)->vmaddr);
	else
		res = swap32(f, ((struct segment_command *) seg)->vmaddr);
	return (res);
}

uint64_t	segment_fileoff(t_machof *f, void *seg)
{
	uint64_t	res;

	if (f->is_64bit)
		res = swap64(f, ((struct segment_command_64 *)seg)->fileoff);
	else
		res = swap32(f, ((struct segment_command *) seg)->fileoff);
	return (res);
}

/*
uint64_t	section_addr(t_machof *f, void *sect)
{
	uint64_t	res;

	if (f->is_64bit)
		res = swap64(f, ((struct section_64 *)sect)->addr);
	else
		res = swap32(f, ((struct section *) sect)->addr);
	ft_printf("section address = %016llx\n", res);
	return (res);
}
*/
/*
** offset in section_64 is a uint32_t !!!
*/
/*
uint64_t	section_offset(t_machof *f, void *sect)
{
	uint64_t	res;

	if (f->is_64bit)
		res = swap32(f, ((struct section_64 *)sect)->offset);
	else
		res = swap32(f, ((struct section *) sect)->offset);
	ft_printf("section offset = %016llx\n", res);
	return (res);
}
*/

void	print_symbol(t_machof *f, uint64_t addr, char sym, char *name)
{
	if (f->is_64bit)
	{
		if (addr != 0)
			ft_printf("%016llx %c %s\n", addr, sym, name);
		else
			ft_printf("%16s %c %s\n", "", sym, name);
	}
	else
	{
		if (addr != 0)
			ft_printf("%08x %c %s\n", addr, sym, name);
		else
			ft_printf("%8s %c %s\n", "", sym, name);
	}
}

void	print_symptr_array(t_machof *f, t_stc *stc, t_nlist **a, uint32_t n)
{
	uint32_t	i;
	uint32_t	n_strx;
	uint32_t	stroff;
	char		*symname;
	uint64_t	symaddr;
	char		symchar;
	void		*seg;

	stroff = swap32(f, stc->stroff);
	i = 0;
	while (i < n)
	{
		n_strx = swap32(f, a[i]->n_un.n_strx);
		symname = (char *)&f->map[stroff + n_strx];
		symaddr = nlist_n_value(f, a[i]);
		symchar = 'U';
		if (a[i]->n_sect != 0)
		{
			seg = get_segment_by_sect_number(f, a[i]->n_sect);
			symaddr += segment_vmaddr(f, seg);
			symaddr -= segment_fileoff(f, seg);
			symchar = get_character_for_symbol(f, a[i]->n_sect);
		}
		print_symbol(f, symaddr, symchar, symname);
		i++;
	}
}

void	nm_print(t_machof *f, t_stc *stc)
{
	uint32_t		count;
	struct nlist	**array;

	count = count_symbols_to_display(f, stc);
	array = ft_memalloc((count + 1) * sizeof_nlist(f));
	array = populate_symptr_array(f, stc, array);
	array = sort_symptr_array(f, stc, array, count);
	(void)print_symptr_array(f, stc, array, count);
	free(array);
	return ;
}


/*
int	nm_symtab(t_machof *f)
{

//	ft_printf("count_symbols_to_display() = %u\n", count_symbols_to_display(f, f->stc));

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

	symoff = swap32(f, f->stc->symoff);
	nsyms = swap32(f, f->stc->nsyms);
	stroff = swap32(f, f->stc->stroff);
	strsize = swap32(f, f->stc->strsize);
	symsize = sizeof_nlist(f);
	i = 0;

	while (i < nsyms)
	{
		nlist = (struct nlist *)&f->map[symoff + i * symsize];
		n_strx = swap32(f, nlist->n_un.n_strx);
		symname = (char *)&f->map[stroff + n_strx];

//		if ((n_strx != 0) && (*symname != 0))
//		{

		uint8_t type = nlist->n_type;
		uint8_t sect = nlist->n_sect;
		uint16_t desc = swap16(f, nlist->n_desc);
		uint64_t value;
		void	*section;

		if (f->is_64bit)
			value = swap64(f, nlist->n_value);
		else
			value = swap32(f, nlist->n_value);


		char rep = '@';

		if (sect != 0)
		{
			rep = get_character_for_symbol(f, sect);

			section = get_segment_by_sect_number(f, sect);

			uint64_t valueX;

			if (f->is_64bit)
				valueX = swap64(f, ((struct segment_command_64 *)section)->vmaddr );
			else
				valueX = swap32(f, ((struct segment_command *) section)->vmaddr);
		//	ft_printf("valueX = %016llx, ", valueX);
			value += valueX;

		} else {
			rep = 'U';
		}
		

//			if (rep != '$')
			if (!(type & N_STAB))
			{
				if (debug)
					ft_printf("index = %08x, n_strx = %08x, type = %02hhx, sect = %02hhx, desc = %04hx\n", i, n_strx, type, sect, desc);
				if (f->is_64bit)
				{
					if (value != 0)
						ft_printf("%016llx %c %s\n", value, rep, symname);
					else

						ft_printf("%16s %c %s\n", "", rep, symname);

				}
				else
					ft_printf("value = %08x, %c name = %s\n", value, rep, symname);
			}

//		}

		i++;
	}


	return (0);
}
*/


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
//		(void)nm_symtab(f);
		(void)nm_print(f, stc);

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
