/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   common.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asarandi <asarandi@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/17 02:19:41 by asarandi          #+#    #+#             */
/*   Updated: 2018/11/19 07:39:38 by asarandi         ###   ########.fr       */
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

#define E_OPEN_ERR		"open() failed"
#define E_FSTAT_ERR		"fstat() failed"
#define E_MMAP_ERR		"mmap() failed"
#define E_MUNMAP_ERR	"munmap() failed"
#define E_FILE_EMPTY	"file is empty"
#define E_BADMAGIC_ERR	"not a valid mach-o file"
#define E_FNOTREG_ERR	"not a regular file"
#define E_BADOFFSET_ERR	"invalid values in header"


typedef struct load_command t_lc;

typedef	struct				s_machof
{
	char					*fn;
	int						fd;
	struct stat				st;
	void					*map;
	int						is_64bit;
	int						is_swapped;
	struct mach_header		*mh;
	uint32_t				ncmds;
	struct symtab_command	*stc;
}							t_machof;

int			fclose_msgerr(int fd, char *s1, char *s2);
int			is_valid_magic(void *map, int *is_64bit, int *is_swapped);
int			msgerr(char *err, char *fn);
int			show_usage(char *s1);
uint16_t	swap16(t_machof *f, uint16_t x);
uint32_t	swap32(t_machof *f, uint32_t x);
uint64_t	swap64(t_machof *f, uint64_t x);
uint32_t	nsects_in_segment(t_machof *f, void *segment);
uint32_t	sizeof_mach_header(t_machof *f);
uint32_t	sizeof_nlist(t_machof *f);
uint32_t	sizeof_section(t_machof *f);
uint32_t	sizeof_segment(t_machof *f);
t_lc		*get_lcmd_by_index(t_machof *f, uint32_t cmd, uint32_t idx);
void		*get_section_by_name_idx(t_machof *f, void *seg, char *sn, uint32_t i);
void		*get_segment_by_sect_number(t_machof *f, uint8_t sect);
void		*get_segment_by_name_idx(t_machof *f, char *segname, uint32_t idx);
void		*get_section_by_number(t_machof *f, uint8_t sect);


#endif
