/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   common.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asarandi <asarandi@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/17 02:19:41 by asarandi          #+#    #+#             */
/*   Updated: 2018/11/23 19:56:47 by asarandi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NM_OTOOL_COMMON_H
# define NM_OTOOL_COMMON_H

#include "libft.h"
#include "libftprintf.h"
#include "functions.h"
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
#define ARCH_X86_64			"x86_64"
#define ARCH_I386			"i386"
#define	ARCH_ARMV7			"armv7"
#define ARCH_ARMV7S			"armv7s"
#define ARCH_ARM64			"arm64"
#define EMPTY_STRING		""
#define	HOST_CPU_TYPE		0x01000007
#define HOST_CPU_SUBTYPE	0x80000003
#define	HOST_ARCH			ARCH_X86_64

/*

e1z4r13p7% ../ft_nm hello_7_arches|grep 'ARCH'
ARCH: hello_7_arches
ARCH: cputype = 00000012, cpusubtype = 0000000a, offset = 0000000000001000, size = 000000000006e008, align = 0000000c
ARCH: cputype = 00000007, cpusubtype = 00000003, offset = 0000000000070000, size = 00000000000020e0, align = 0000000c
ARCH: cputype = 01000007, cpusubtype = 80000003, offset = 0000000000073000, size = 00000000000020f0, align = 0000000c
ARCH: cputype = 0000000c, cpusubtype = 00000000, offset = 0000000000078000, size = 00000000000857d0, align = 0000000e
ARCH: cputype = 0000000c, cpusubtype = 00000009, offset = 0000000000100000, size = 000000000000c0ec, align = 0000000e
ARCH: cputype = 0000000c, cpusubtype = 0000000b, offset = 0000000000110000, size = 000000000000c0ec, align = 0000000e
ARCH: cputype = 0100000c, cpusubtype = 00000000, offset = 0000000000120000, size = 000000000000c0f0, align = 0000000e
e1z4r13p7% file hello_7_arches            
hello_7_arches: Mach-O universal binary with 7 architectures: [ppc_7400:Mach-O executable ppc_7400]

hello_7_arches (for architecture ppc7400):	Mach-O executable ppc_7400
hello_7_arches (for architecture i386):	Mach-O executable i386
hello_7_arches (for architecture x86_64):	Mach-O 64-bit executable x86_64
hello_7_arches (for architecture arm):	Mach-O executable arm
hello_7_arches (for architecture armv7):	Mach-O executable arm_v7
hello_7_arches (for architecture armv7s):	Mach-O executable arm_v7s
hello_7_arches (for architecture arm64):	Mach-O 64-bit executable arm64

*/

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

typedef struct				s_file
{
	int						print_names;
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
	uint32_t				cputype;
	uint32_t				cpusubtype;
	char					*fn;
	char					*arch;
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
