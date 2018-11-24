/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getters.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asarandi <asarandi@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/18 10:11:31 by asarandi          #+#    #+#             */
/*   Updated: 2018/11/23 19:19:38 by asarandi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "common.h"

/*
** returns matching load command by index or NULL
** for example 2nd LC_SEGMENT or first LC_SYMTAB
*/

t_lc	*get_lcmd_by_index(t_bin *b, uint32_t cmd, uint32_t idx)
{
	uint32_t	i;
	uint32_t	counter;
	off_t		offset;
	t_lc		*lcmd;

	i = 0;
	counter = 0;
	offset = sizeof_mach_header(b);
	while (i < b->ncmds)
	{
		lcmd = (struct load_command *)&b->data[offset];
		if (swap32(b, lcmd->cmd) == cmd)
		{
			if (counter == idx)
				return (lcmd);
			counter++;
		}
		offset += swap32(b, lcmd->cmdsize);
		i++;
	}
	return (NULL);
}

/*
** search macho for a segment with a specific name
** index is used in case there's multiple occurances of same seg name
*/

void	*get_segment_by_name_idx(t_bin *b, char *segname, uint32_t idx)
{
	uint32_t	lctype;
	void		*segment;
	uint32_t	iseg;
	uint32_t	matches;

	lctype = LC_SEGMENT_64;
	if (b->is_64bit == 0)
		lctype = LC_SEGMENT;
	iseg = 0;
	matches = 0;
	while ((segment = get_lcmd_by_index(b, lctype, iseg++)) != NULL)
	{
		if ((ft_strcmp(((struct segment_command *)segment)->segname,
					segname)) == 0)
		{
			if (matches == idx)
				return (segment);
			matches++;
		}
	}
	return (NULL);
}

void	*get_section_by_name_idx(t_bin *b, void *seg, char *sn, uint32_t i)
{
	uint32_t	matches;
	uint32_t	nsects;
	uint32_t	k;

	nsects = nsects_in_segment(b, seg);
	seg += sizeof_segment(b);
	k = 0;
	matches = 0;
	while (k < nsects)
	{
		if (ft_strcmp(((struct section *)seg)->sectname, sn) == 0)
		{
			if (matches == i)
				return (seg);
			matches++;
		}
		seg += sizeof_section(b);
		k++;
	}
	return (NULL);
}

/*
** this is for getting the vmaddr of a segment
** which will be added to value in nlist for a symbol
*/

void	*get_segment_by_sect_number(t_bin *b, uint8_t sect)
{
	uint32_t	lctype;
	void		*segment;
	uint32_t	iseg;
	uint32_t	nsects;
	uint32_t	total_sects;

	lctype = load_command_segment(b);
	iseg = 0;
	total_sects = 0;
	while ((segment = get_lcmd_by_index(b, lctype, iseg++)) != NULL)
	{
		nsects = nsects_in_segment(b, segment);
		if (nsects > 0)
		{
			if ((sect >= total_sects) && (sect <= total_sects + nsects))
				return (segment);
			total_sects += nsects;
		}
	}
	return (NULL);
}

/*
** this goes through all sections in the macho
** finds the segment that contains section #n,
** then goes to that particular section
** returns (void *) which is
** either (struct section *) or (struct section_64 *)
** this function is for devising the character that
** represents the symbol in symtable based on the
** section number in nload
*/

void	*get_section_by_number(t_bin *b, uint8_t sect)
{
	uint32_t	lctype;
	void		*segment;
	uint32_t	iseg;
	uint32_t	nsects;
	uint32_t	total_sects;

	lctype = load_command_segment(b);
	iseg = 0;
	total_sects = 0;
	while ((segment = get_lcmd_by_index(b, lctype, iseg++)) != NULL)
	{
		nsects = nsects_in_segment(b, segment);
		if (nsects > 0)
		{
			if ((sect >= total_sects) && (sect <= total_sects + nsects))
			{
				segment += sizeof_segment(b);
				segment += sizeof_section(b) * (sect - total_sects - 1);
				return (segment);
			}
			total_sects += nsects;
		}
	}
	return (NULL);
}
