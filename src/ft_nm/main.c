/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asarandi <asarandi@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/16 01:02:15 by asarandi          #+#    #+#             */
/*   Updated: 2018/11/17 08:17:02 by asarandi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"
#include <mach-o/loader.h>
#include <mach-o/nlist.h>

#define E_OPEN_ERR		"open() failed"
#define E_FSTAT_ERR		"fstat() failed"
#define E_MMAP_ERR		"mmap() failed"
#define E_MUNMAP_ERR	"munmap() failed"
#define E_FILE_EMPTY	"file is empty"
#define E_BADMAGIC_ERR	"not a valid mach-o file"
#define E_FNOTREG_ERR	"not a regular file"

int	show_usage(char *s1)
{
	ft_printf("usage: %s file\n", s1);
	return (0);
}

int	msgerr(char *err, char *fn)
{
	ft_fprintf(STDERR_FILENO,
			"{red}error{eoc} [{yellow}%s{eoc}]: %s\n", err, fn);
	return (0);
}

int	fclose_msgerr(int fd, char *s1, char *s2)
{
	(void)close(fd);
	return (msgerr(s1, s2));
}

int	is_valid_magic(void *map, int *is_64bit, int *is_swapped)
{
	struct mach_header	*mh;

	mh = (struct mach_header *)map;
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


uint16_t	swap16(int is_swapped, uint16_t x)
{
	uint16_t	res;

	res = 0;
	if (is_swapped)
	{
		res += (x >> 0x08) & 0x00ff;
		res += (x << 0x08) & 0xff00;
	}
	else
		res = x;
	return (res);
}

uint32_t	swap32(int is_swapped, uint32_t x)
{
	uint32_t	res;

	res = 0;
	if (is_swapped)
	{
		res += (x >> 0x18) & 0x000000ff;
		res += (x >> 0x08) & 0x0000ff00;
		res += (x << 0x08) & 0x00ff0000;
		res += (x << 0x18) & 0xff000000;
	}
	else
		res = x;
	return (res);
}

uint64_t	swap64(int is_swapped, uint64_t x)
{
	uint64_t	res;

	res = 0;
	if (is_swapped)
	{
		res += (x >> 0x38) & 0x00000000000000ff;
		res += (x >> 0x28) & 0x000000000000ff00;
		res += (x >> 0x18) & 0x0000000000ff0000;
		res += (x >> 0x08) & 0x00000000ff000000;
		res += (x << 0x08) & 0x000000ff00000000;
		res += (x << 0x18) & 0x0000ff0000000000;
		res += (x << 0x28) & 0x00ff000000000000;
		res += (x << 0x38) & 0xff00000000000000;
	}
	else
		res = x;
	return (res);
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


int	nm_symtab(char *fn, void *map, off_t fsize, struct symtab_command *stc, int is_swapped, int is_64bit)
{
	(void)fn;
	(void)fsize;
	(void)map;
	ft_printf("    cmd = %08x\n", swap32(is_swapped, stc->cmd));
	ft_printf("cmdsize = %08x\n", swap32(is_swapped, stc->cmdsize));
	ft_printf(" symoff = %08x\n", swap32(is_swapped, stc->symoff));
	ft_printf("  nsyms = %08x\n", swap32(is_swapped, stc->nsyms));
	ft_printf(" stroff = %08x\n", swap32(is_swapped, stc->stroff));
	ft_printf("strsize = %08x\n", swap32(is_swapped, stc->strsize));


	uint32_t	symoff;
	uint32_t	nsyms;
	uint32_t	stroff;
	uint32_t	strsize;
	uint32_t	i;
	symoff = swap32(is_swapped, stc->symoff);
	nsyms = swap32(is_swapped, stc->nsyms);
	stroff = swap32(is_swapped, stc->stroff);
	strsize = swap32(is_swapped, stc->strsize);
	i = 0;

	struct nlist	*nlist;
	ssize_t symsize;
	symsize = (is_64bit == 1) ? sizeof(struct nlist_64) : sizeof(struct nlist);

	while (i < nsyms)
	{
		nlist = (struct nlist *)&map[symoff + i * symsize];
		uint32_t n_strx = swap32(is_swapped, nlist->n_un.n_strx);
		char *symname = (char *)&map[stroff + n_strx];


		uint8_t type = nlist->n_type;
		uint8_t sect = nlist->n_sect;
		uint16_t desc = swap16(is_swapped, nlist->n_desc);
		uint64_t val64;
		uint32_t val32;

		if (is_64bit)
			val64 = swap64(is_swapped, nlist->n_value);
		else
			val32 = swap32(is_swapped, nlist->n_value);



		ft_printf("type = %02hhx, sect = %02hhx, desc = %04hx, ", type, sect, desc);
		if (is_64bit)
			ft_printf("value = %016llx, name = %s\n", val64, symname);
		else
			ft_printf("value = %08x, name = %s\n", val32, symname);

		i++;
	}


	return (0);
}

int	nm(char *fn, void *map, off_t fsize)
{
	int					is_64bit;
	int					is_swapped;
	uint32_t			ncmds;
	uint32_t			i;
	off_t				offset;
	struct load_command	*lc;
	struct mach_header	*mh;
	struct symtab_command *stc;

//struct symtab_command {
//	uint32_t	cmd;		/* LC_SYMTAB */
//	uint32_t	cmdsize;	/* sizeof(struct symtab_command) */
//	uint32_t	symoff;		/* symbol table offset */
//	uint32_t	nsyms;		/* number of symbol table entries */
//	uint32_t	stroff;		/* string table offset */
//	uint32_t	strsize;	/* string table size in bytes */
//};


	if (!is_valid_magic(map, &is_64bit, &is_swapped))
		return (msgerr(E_BADMAGIC_ERR, fn));
	mh = (struct mach_header *)map;
	ncmds = swap32(is_swapped, mh->ncmds);

	offset = (is_64bit == 1) ?
		sizeof(struct mach_header_64) : sizeof(struct mach_header); 
	i = 0;
	while ((i < ncmds) && (offset < fsize))
	{
		lc = (struct load_command *)&map[offset];
		if (swap32(is_swapped, lc->cmd) == LC_SYMTAB)
		{
			stc = (struct symtab_command *)&map[offset];
			ft_printf("LC_SYMTAB command at %llx\n", offset);


			(void)nm_symtab(fn, map, fsize, stc, is_swapped, is_64bit);
		}
		offset += swap32(is_swapped, lc->cmdsize);
		i++;
	}
	return (0);
}

int	process_file(char *fn)
{
	int				fd;
	struct stat		st;
	void			*map;

	if ((fd = open(fn, O_RDONLY)) == -1)
		return (msgerr(E_OPEN_ERR, fn));
	if (fstat(fd, &st) == -1)
		return (fclose_msgerr(fd, E_FSTAT_ERR, fn));
	if (st.st_size < 1)
		return (fclose_msgerr(fd, E_FILE_EMPTY, fn));
	if (!(st.st_mode & S_IFREG))
		return (fclose_msgerr(fd, E_FNOTREG_ERR, fn));
	map = mmap(0, st.st_size, PROT_READ, MAP_FILE | MAP_SHARED, fd, 0);
	if (map == MAP_FAILED)
		return (fclose_msgerr(fd, E_MMAP_ERR, fn));
	(void)nm(fn, map, st.st_size);
	if (munmap(map, st.st_size) == -1)
		return (fclose_msgerr(fd, E_MUNMAP_ERR, fn));
	(void)close(fd);
	return (0);
}

int main(int ac, char **av)
{
	int	i;

	i = 1;
	while (i < ac)
		(void)process_file(av[i++]);
	if (i == 1)
		(void)show_usage(av[0]);
	return (0);
}
