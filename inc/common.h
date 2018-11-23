/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   common.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asarandi <asarandi@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/17 02:19:41 by asarandi          #+#    #+#             */
/*   Updated: 2018/11/23 13:15:57 by asarandi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NM_OTOOL_COMMON_H
# define NM_OTOOL_COMMON_H

#include "libft.h"
#include "libftprintf.h"
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <mach-o/loader.h>
#include <mach-o/nlist.h>
#include <mach-o/fat.h>
#include <ar.h>
#include <mach-o/ranlib.h>

#define E_OPEN_ERR		"open() failed"
#define E_FSTAT_ERR		"fstat() failed"
#define E_MMAP_ERR		"mmap() failed"
#define E_MUNMAP_ERR		"munmap() failed"
#define E_FILE_EMPTY		"file is empty"
#define E_BADMAGIC_ERR		"not a valid mach-o file"
#define E_FNOTREG_ERR		"not a regular file"
#define E_BADOFFSET_ERR		"invalid values in header"
#define E_BADFTYPE_ERR		"unsupported file type"
#define E_ARBADFMAG			"bad fmag in archive"

#ifndef FAT_MAGIC_64
# define FAT_MAGIC_64  0xcafebabf

struct fat_arch_64 {
	cpu_type_t	cputype;	/* cpu specifier (int) */
	cpu_subtype_t	cpusubtype;	/* machine specifier (int) */
	uint64_t	offset;		/* file offset to this object file */
	uint64_t	size;		/* size of this object file */
	uint32_t	align;		/* alignment as a power of 2 */
	uint32_t	reserved;	/* reserved */
};

#endif

#ifndef FAT_CIGAM_64
# define FAT_CIGAM_64  0xbfbafeca
#endif


typedef struct load_command		t_lc;
typedef struct symtab_command	t_stc;
typedef struct nlist			t_nlist;

typedef struct				t_file
{
	char					*fn;
	int						fd;
	struct stat				st;
	void					*map;
	int						is_archive;
	int						is_fat;
	int						is_fat64;
	int						is_swapped;
}							t_file;

typedef	struct				s_bin
{
	t_file					*parent;
	char					*fn;
	off_t					fsize;
	void					*data;
	int						is_64bit;
	int						is_swapped;
	struct mach_header		*mh;
	uint32_t				ncmds;
	struct symtab_command	*stc;
}							t_bin;



int			fclose_msgerr(int fd, char *s1, char *s2);
int			is_valid_magic(void *map, int *is_64bit, int *is_swapped);
int			msgerr(char *err, char *fn);
int			show_usage(char *s1);
uint16_t	swap16u(uint16_t x);
uint32_t	swap32u(uint32_t x);
uint64_t	swap64u(uint64_t x);
uint16_t	swap16(t_bin *b, uint16_t x);
uint32_t	swap32(t_bin *b, uint32_t x);
uint64_t	swap64(t_bin *b, uint64_t x);
uint32_t	nsects_in_segment(t_bin *b, void *segment);
uint32_t	sizeof_mach_header(t_bin *b);
uint32_t	sizeof_nlist(t_bin *b);
uint32_t	sizeof_section(t_bin *b);
uint32_t	sizeof_segment(t_bin *b);
uint64_t	nlist_n_value(t_bin *b, t_nlist *nlist);
uint64_t	segment_fileoff(t_bin *b, void *seg);
uint64_t	segment_vmaddr(t_bin *b, void *seg);
t_lc		*get_lcmd_by_index(t_bin *b, uint32_t cmd, uint32_t idx);
void		*get_section_by_name_idx(t_bin *b, void *seg, char *sn, uint32_t i);
void		*get_segment_by_sect_number(t_bin *b, uint8_t sect);
void		*get_segment_by_name_idx(t_bin *b, char *segname, uint32_t idx);
void		*get_section_by_number(t_bin *b, uint8_t sect);


#endif
