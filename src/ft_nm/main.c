/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asarandi <asarandi@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/16 01:02:15 by asarandi          #+#    #+#             */
/*   Updated: 2018/11/22 22:30:23 by asarandi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

char	get_symchar_from_section(t_bin *b, uint8_t n_sect)
{
	void	*sect;
	char	*sectname;
	char	*segname;

	sect = get_section_by_number(b, n_sect);
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

t_nlist	**populate_symptr_array(t_bin *b, t_stc *stc, t_nlist **array)
{
	uint32_t		i;
	uint32_t		j;
	uint32_t		symoff;
	struct nlist	*nlist;

	symoff = swap32(b, stc->symoff);
	i = 0;
	j = 0;
	while (i < swap32(b, stc->nsyms))
	{
		nlist = (struct nlist *)&b->data[symoff + i * sizeof_nlist(b)];
		if (!(nlist->n_type & N_STAB))
		{
			array[j++] = nlist;
		}
		i++;
	}
	array[j] = NULL;
	return (array);
}

uint32_t	count_symbols_to_display(t_bin *b, t_stc *stc)
{
	uint32_t		i;
	uint32_t		count;
	uint32_t		symoff;
	struct nlist	*nlist;

	symoff = swap32(b, stc->symoff);
	i = 0;
	count = 0;
	while (i < swap32(b, stc->nsyms))
	{
		nlist = (struct nlist *)&b->data[symoff + i * sizeof_nlist(b)];
		if (!(nlist->n_type & N_STAB))
			count++;
		i++;
	}
	return (count);
}


int		compare_symbols(t_bin *b, t_stc *stc, t_nlist *sym1, t_nlist *sym2)
{
	uint32_t	stroff;
	uint32_t	n_strx;
	char		*sym1_name;
	char		*sym2_name;
	int			res;

	stroff = swap32(b, stc->stroff);
	n_strx = swap32(b, sym1->n_un.n_strx);
	sym1_name = (char *)&b->data[stroff + n_strx];
	n_strx = swap32(b, sym2->n_un.n_strx);
	sym2_name = (char *)&b->data[stroff + n_strx];
	res = ft_strcmp(sym1_name, sym2_name);
	if (!res)
		res = (int)(nlist_n_value(b, sym1) - nlist_n_value(b, sym2));
	return (res);

}

/*
** https://en.wikipedia.org/wiki/Bubble_sort
*/

t_nlist	**sort_symptr_array(t_bin *b, t_stc *stc, t_nlist **array, uint32_t n)
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
			if (compare_symbols(b, stc, array[i - 1], array[i]) > 0)
			{
				tmp = array[i - 1];
				array[i - 1] = array[i];
				array[i] = tmp;
				swapped = 1;
			}
			i++;
		}
		if (!swapped)
			break ;
	}
	return (array);
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







void	print_symbol(t_bin *b, uint64_t addr, char sym, char *name)
{
	if (b->is_64bit)
	{
		if (print_address_for_symbol(sym) == 1)
			ft_printf("%016llx %c %s", addr, sym, name);
		else
			ft_printf("%16s %c %s", "", sym, name);
	}
	else
	{
		if (print_address_for_symbol(sym) == 1)
			ft_printf("%08llx %c %s", addr & 0xffffffff, sym, name);
		else
			ft_printf("%8s %c %s", "", sym, name);
	}
	if (sym == 'I')
		ft_printf(" (indirect for %s)", name);
	ft_printf("\n");
}

char	get_symchar_from_ntype(t_bin *b, t_nlist *sym)
{
	char	symchar;

	if ((sym->n_type & N_TYPE) == N_UNDF)
		symchar = (nlist_n_value(b, sym) == 0) ? 'u' : 'c';
	else if ((sym->n_type & N_TYPE) == N_PBUD)
		symchar = 'u';
	else if ((sym->n_type & N_TYPE) == N_ABS)
		symchar = 'a';
	else if ((sym->n_type & N_TYPE) == N_SECT)
		symchar = get_symchar_from_section(b, sym->n_sect);
	else if ((sym->n_type & N_TYPE) == N_INDR)
		symchar = 'i';
	else
		symchar = '?';
	if ((sym->n_type & N_EXT) && (symchar != '?'))
		symchar = ft_toupper(symchar);
	return (symchar);
}

char	*get_symname_from_nlist(t_bin *b, t_stc *stc, t_nlist *sym)
{
	uint32_t	n_strx;
	uint32_t	stroff;
	char		*symname;

	stroff = swap32(b, stc->stroff);
	n_strx = swap32(b, sym->n_un.n_strx);
	symname = (char *)&b->data[stroff + n_strx];
	return (symname);
}

uint64_t	get_symaddr_from_nlist(t_bin *b, t_nlist *sym)
{
	uint64_t	symaddr;
	void		*seg;

	symaddr = nlist_n_value(b, sym);
	if (((sym->n_type & N_TYPE) & N_SECT) && (b->is_64bit == 1))
	{
		seg = get_segment_by_sect_number(b, sym->n_sect);
		if (segment_vmaddr(b, seg) != 0)
		{
			symaddr += segment_vmaddr(b, seg);
			symaddr -= segment_fileoff(b, seg);
		}
	}
	return (symaddr);
}

void	print_symptr_array(t_bin *b, t_stc *stc, t_nlist **array, uint32_t n)
{
	uint32_t	i;
	char		*symname;
	uint64_t	symaddr;
	char		symchar;

	i = 0;
	while (i < n)
	{
		symname = get_symname_from_nlist(b, stc, array[i]);
		symchar = get_symchar_from_ntype(b, array[i]);
		symaddr = get_symaddr_from_nlist(b, array[i]);
		print_symbol(b, symaddr, symchar, symname);
		i++;
	}
}

void	nm_print(t_bin *b, t_stc *stc)
{
	uint32_t		count;
	struct nlist	**array;

	count = count_symbols_to_display(b, stc);
	array = ft_memalloc((count + 1) * sizeof_nlist(b));
	array = populate_symptr_array(b, stc, array);
	array = sort_symptr_array(b, stc, array, count);
	(void)print_symptr_array(b, stc, array, count);
	free(array);
	return ;
}


/*
** validate the magic and check load commands sizes;
** returns 0 on success, 1 on failure (prints error msg)
** will populate (t_bin *) with values for other functions
*/


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

int	process_macho(t_bin *b)
{
	uint32_t				isym;
	struct symtab_command	*stc;
	struct load_command		*lcmd;

	if (validate_macho(b) != 0)
		return (1);
	isym = 0;
	while ((lcmd = get_lcmd_by_index(b, LC_SYMTAB, isym++)) != NULL)
	{
		stc = (struct symtab_command *)lcmd;
		b->stc = stc;
		(void)nm_print(b, stc);
	}
	return (0);
}

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
	if (ft_strcmp(ARCHIVE_MAGIC, (char *)f->map) == 0)
		f->is_archive = 1;
	return (f->is_archive);
}



/*
** pick file to process
*/

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


int	print_arch_info(t_file *f, void *fat_arch)
{
	uint32_t	cputype;	/* XXX integer_t, uint32_t is wrong */
	uint32_t	cpusubtype;
	uint64_t	offset;
	uint64_t	size;
	uint32_t	align;

	if (f->is_swapped)
		cputype = swap32u(((struct fat_arch *)fat_arch)->cputype);
	else
		cputype = ((struct fat_arch *)fat_arch)->cputype;
	if (f->is_swapped)
		cpusubtype = swap32u(((struct fat_arch *)fat_arch)->cpusubtype);
	else
		cpusubtype = ((struct fat_arch *)fat_arch)->cpusubtype;
	if (f->is_fat64)
	{
		if (f->is_swapped)
		{
			offset = swap64u(((struct fat_arch_64 *)fat_arch)->offset);
			size   = swap64u(((struct fat_arch_64 *)fat_arch)->size);
			align  = swap32u(((struct fat_arch_64 *)fat_arch)->align);
		}
		else
		{
			offset = ((struct fat_arch_64 *)fat_arch)->offset;
			size   = ((struct fat_arch_64 *)fat_arch)->size;
			align  = ((struct fat_arch_64 *)fat_arch)->align;
		}
	}
	else
	{
		if (f->is_swapped)
		{
			offset = swap32u(((struct fat_arch *)fat_arch)->offset);
			size   = swap32u(((struct fat_arch *)fat_arch)->size);
			align  = swap32u(((struct fat_arch *)fat_arch)->align);
		}
		else
		{
			offset = ((struct fat_arch *)fat_arch)->offset;
			size   = ((struct fat_arch *)fat_arch)->size;
			align  = ((struct fat_arch *)fat_arch)->align;
		}
	}
	ft_printf("ARCH: cputype = %08x, cpusubtype = %08x, offset = %016llx, size = %016llx, align = %08x\n",
			cputype, cpusubtype, offset, size, align);
	return (0);
}


//bin/sync:       Mach-O universal binary with 2 architectures: [x86_64:Mach-O 64-bit executable x86_64] [i386:Mach-O executable i386]
//ARCH: cputype = 01000007, cpusubtype = 80000003, offset = 0000000000001000, size = 00000000000045b0, align = 0000000c



uint32_t	fat_arch_cputype(t_file *f, void *fat_arch)
{
	if (f->is_swapped)
		return (swap32u(((struct fat_arch *)fat_arch)->cputype));
	else
		return (((struct fat_arch *)fat_arch)->cputype);
}

uint32_t	fat_arch_cpusubtype(t_file *f, void *fat_arch)
{
	if (f->is_swapped)
		return (swap32u(((struct fat_arch *)fat_arch)->cpusubtype));
	else
		return (((struct fat_arch *)fat_arch)->cpusubtype);
}

int	fat_file_choose_arch(t_file *f)
{
	uint32_t	i;
	uint32_t	second_choice;
	void		*fat_arch;

	i = 0;
	second_choice = 0;
	while (i < file_nfat_arch(f))
	{
		fat_arch = &f->map[
			sizeof(struct fat_header) + (sizeof_fat_arch(f) * i)];
		if ((fat_arch_cputype(f, fat_arch) & CPU_TYPE_X86_64) &&
			(fat_arch_cpusubtype(f, fat_arch) & CPU_SUBTYPE_X86_64_ALL))
			return (i);
		if (fat_arch_cputype(f, fat_arch) & CPU_ARCH_ABI64)
			second_choice = i;
		i++;
	}
	return (second_choice);
}

void fat_print_arches(t_file *f)
{
	uint32_t	i;
	void		*fat_arch;

	ft_printf("ARCH: %s\n", f->fn);
	i = 0;
	while (i < file_nfat_arch(f))
	{
		fat_arch = &f->map[
			sizeof(struct fat_header) + (sizeof_fat_arch(f) * i)];
		(void)print_arch_info(f, fat_arch);

		i++;
	}
	return ;
}

int	fat_file_loader(t_file *f)
{
	t_bin		b;
	uint32_t	i;
	void		*fat_arch;

	(void)fat_print_arches(f);

	i = fat_file_choose_arch(f);
	ft_memset(&b, 0, sizeof(t_bin));
	fat_arch = &f->map[sizeof(struct fat_header) + (sizeof_fat_arch(f) * i)];
	b.data = &f->map[fat_arch_offset(f, fat_arch)];
	b.fsize = fat_arch_size(f, fat_arch);
	b.fn = NULL;
	process_macho(&b);
	return (0);
}

int	archive_file_loader(t_file *f)
{
	(void)f;
	//parse header
	//iterate through all objects
	//send to process_macho
	return (0);
}

int	binary_loader(t_file *f)
{
	t_bin		b;

	if (is_macho_file(f))				//standalone macho
	{
		(void)ft_memset(&b, 0, sizeof(t_bin));
		b.data = f->map;
		b.fn = f->fn;
		b.fsize = f->st.st_size;
		return (process_macho(&b));
	}
	else if (is_fat_file(f))			//multi-arch, fat binary
		return (fat_file_loader(f));

	else if (is_archive_file(f))		//archive, like libft.a
		return (archive_file_loader(f));
	else
		return (fclose_msgerr(f->fd, E_BADFTYPE_ERR, f->fn));
}

int	process_file(t_file *f)
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
	(void)binary_loader(f);
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
