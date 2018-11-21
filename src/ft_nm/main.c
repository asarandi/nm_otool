/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asarandi <asarandi@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/16 01:02:15 by asarandi          #+#    #+#             */
/*   Updated: 2018/11/21 13:07:05 by asarandi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

char	get_symchar_from_section(t_machof *f, uint8_t n_sect)
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
			return ('t');
		if ((ft_strcmp(segname, SEG_DATA) == 0) &&		//SEG_DATA
				(ft_strcmp(sectname, SECT_DATA) == 0))	//SECT_DATA
			return ('d');
		if ((ft_strcmp(segname, SEG_DATA) == 0) &&		//SEG_DATA
				(ft_strcmp(sectname, SECT_BSS) == 0))	//SECT_BSS
			return ('b');
	}
	return ('s');
}

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
		nlist = (struct nlist *)&f->single[symoff + i * sizeof_nlist(f)];
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
		nlist = (struct nlist *)&f->single[symoff + i * sizeof_nlist(f)];
		if (!(nlist->n_type & N_STAB))
			count++;
		i++;
	}
	return (count);
}


int		compare_symbols(t_machof *f, t_stc *stc, t_nlist *a, t_nlist *b)
{
	uint32_t	stroff;
	uint32_t	n_strx;
	char		*a_name;
	char		*b_name;
	int			res;

	stroff = swap32(f, stc->stroff);
	n_strx = swap32(f, a->n_un.n_strx);
	a_name = (char *)&f->single[stroff + n_strx];
	n_strx = swap32(f, b->n_un.n_strx);
	b_name = (char *)&f->single[stroff + n_strx];
	res = ft_strcmp(a_name, b_name);
	if (!res)
		res = (int)(nlist_n_value(f, a) - nlist_n_value(f, b));
	return (res);

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
			if (compare_symbols(f, stc, a[i - 1], a[i]) > 0)
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
//--------------------------------------------------------------------------------------

int		print_address_for_symbol(char c)
{
	if ((c == 'U') || (c == 'u'))
		return (0);
	if ((c == 'I') || (c == 'i'))
		return (0);
	return (1);
}

void	print_symbol(t_machof *f, uint64_t addr, char sym, char *name)
{
	if (f->is_64bit)
	{
		if (print_address_for_symbol(sym) == 1)
			ft_printf("%016llx %c %s\n", addr, sym, name);
		else
			ft_printf("%16s %c %s\n", "", sym, name);
	}
	else
	{
		if (print_address_for_symbol(sym) == 1)
			ft_printf("%08llx %c %s\n", addr & 0xffffffff, sym, name);
		else
			ft_printf("%8s %c %s\n", "", sym, name);
	}
}

char	get_symchar_from_ntype(t_machof *f, t_nlist *sym)
{
	char	symchar;

	if ((sym->n_type & N_TYPE) == N_UNDF)
		symchar = (nlist_n_value(f, sym) == 0) ? 'u' : 'c';
	else if ((sym->n_type & N_TYPE) == N_PBUD)
		symchar = 'u';
	else if ((sym->n_type & N_TYPE) == N_ABS)
		symchar = 'a';
	else if ((sym->n_type & N_TYPE) == N_SECT)
		symchar = get_symchar_from_section(f, sym->n_sect);
	else if ((sym->n_type & N_TYPE) == N_INDR)
		symchar = 'i';
	else
		symchar = '?';
	if ((sym->n_type & N_EXT) && (symchar != '?'))
		symchar = ft_toupper(symchar);
	return (symchar);
}

char	*get_symname_from_nlist(t_machof *f, t_stc *stc, t_nlist *sym)
{
	uint32_t	n_strx;
	uint32_t	stroff;
	char		*symname;

	stroff = swap32(f, stc->stroff);
	n_strx = swap32(f, sym->n_un.n_strx);
	symname = (char *)&f->single[stroff + n_strx];
	return (symname);
}

uint64_t	get_symaddr_from_nlist(t_machof *f, t_nlist *sym)
{
	uint64_t	symaddr;
	void		*seg;

	symaddr = nlist_n_value(f, sym);
	if (((sym->n_type & N_TYPE) & N_SECT) && (f->is_64bit == 1))
	{
		seg = get_segment_by_sect_number(f, sym->n_sect);
		if (segment_vmaddr(f, seg) != 0)
		{
			symaddr += segment_vmaddr(f, seg);
			symaddr -= segment_fileoff(f, seg);
		}
	}
	return (symaddr);
}

void	print_symptr_array(t_machof *f, t_stc *stc, t_nlist **a, uint32_t n)
{
	uint32_t	i;
	char		*symname;
	uint64_t	symaddr;
	char		symchar;

	i = 0;
	while (i < n)
	{
		symname = get_symname_from_nlist(f, stc, a[i]);
		symchar = get_symchar_from_ntype(f, a[i]);
		symaddr = get_symaddr_from_nlist(f, a[i]);
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
** validate the magic and check load commands sizes;
** returns 0 on success, 1 on failure (prints error msg)
** will populate t_machof with values for other functions
*/


int	validate_macho(t_machof *f)
{
	uint32_t			i;
	off_t				offset;
	struct load_command	*lc;


	if (!is_valid_magic(f->single, &f->is_64bit, &f->is_swapped))
		return (msgerr(E_BADMAGIC_ERR, f->fn));
	f->mh = (struct mach_header *)f->single;
	f->ncmds = swap32(f, f->mh->ncmds);
	offset = sizeof_mach_header(f);
	i = 0;
	while ((i < f->ncmds) && (offset < f->st.st_size))
	{
		lc = (struct load_command *)&f->single[offset];
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
		(void)nm_print(f, stc);

	}

	return (0);
}

int	is_macho_file(void *data)
{
	uint32_t	magic;
	magic = ((struct mach_header *)data)->magic;
	if ((magic == MH_MAGIC) || (magic == MG_CIGAM))
		return (1);
	else if ((magic == MH_MAGIC_64) || (magic == MG_CIGAM_64))
		return (1);
	return (0);
}

int	is_fat_file(void *data)
{
	uint32_t	magic;

	magic = ((struct fat_header *)data)->magic;
	if ((magic == FAT_MAGIC) || (magic == FAT_CIGAM))
		return (1);
	else if ((magic == FAT_MAGIC_64) || (magic == FAT_CIGAM_64))
		return (1);
	return (0);
}


#define ARCHIVE_MAGIC "!<arch>\n"
int	is_archive_file(void *data)
{
	if (ft_strcmp(ARCHIVE_MAGIC, (char *)data) == 0)
		return (1);
	return (0);
}

int	file_loader(t_machof *f)
{
	if (is_macho_file(f->multi))
		return (process_macho(f->multi));
	else if (is_fat_file(f->multi))
		return (process_fat_file(f->multi));
	else if (is_archive_file(f->multi))
		return (process_archive_file(f->multi));
	else
		return 





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
	f->multi = mmap(0, f->st.st_size, PROT_READ, MAP_SHARED, f->fd, 0);
	if (f->multi == MAP_FAILED)
		return (fclose_msgerr(f->fd, E_MMAP_ERR, f->fn));
	f->single = f->multi;
	(void)nm(f);
	if (munmap(f->multi, f->st.st_size) == -1)
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
