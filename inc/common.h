/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   common.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asarandi <asarandi@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/17 02:19:41 by asarandi          #+#    #+#             */
/*   Updated: 2018/11/23 23:49:40 by asarandi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NM_OTOOL_COMMON_H
# define NM_OTOOL_COMMON_H

# include "libft.h"
# include "libftprintf.h"
# include "functions.h"
# include <unistd.h>
# include <fcntl.h>
# include <stdlib.h>
# include <sys/stat.h>
# include <sys/mman.h>
# include <mach-o/loader.h>
# include <mach-o/nlist.h>
# include <mach-o/fat.h>
# include <ar.h>
# include <mach-o/ranlib.h>

# define E_OPEN_ERR			"open() failed"
# define E_FSTAT_ERR		"fstat() failed"
# define E_MMAP_ERR			"mmap() failed"
# define E_MUNMAP_ERR		"munmap() failed"
# define E_FILE_EMPTY		"file is empty"
# define E_BADMAGIC_ERR		"not a valid mach-o file"
# define E_FNOTREG_ERR		"not a regular file"
# define E_BADOFFSET_ERR	"invalid values in header"
# define E_BADFTYPE_ERR		"unsupported file type"
# define E_ARBADFMAG		"bad fmag in archive"
# define ARCH_X86_64		"x86_64"
# define ARCH_I386			"i386"
# define ARCH_ARMV7			"armv7"
# define ARCH_ARMV7S		"armv7s"
# define ARCH_ARM64			"arm64"
# define EMPTY_STRING		""
# define HOST_CPU_TYPE		0x01000007
# define HOST_CPU_SUBTYPE	0x80000003
# define HOST_ARCH			ARCH_X86_64

/*
**# ifndef FAT_MAGIC_64
**#  define FAT_MAGIC_64  0xcafebabf
**
**struct	fat_arch_64
**{
**	cpu_type_t		cputype;
**	cpu_subtype_t	cpusubtype;
**	uint64_t		offset;
**	uint64_t		size;
**	uint32_t		align;
**	uint32_t		reserved;
**};
**
**# endif
**
**# ifndef FAT_CIGAM_64
**#  define FAT_CIGAM_64  0xbfbafeca
**# endif
*/

typedef struct load_command		t_lc;
typedef struct symtab_command	t_stc;
typedef struct nlist			t_nlist;
typedef uint32_t				t_u32;

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
	struct nlist			**array;
}							t_bin;

#endif
