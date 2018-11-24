/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm_segments.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asarandi <asarandi@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/23 22:35:25 by asarandi          #+#    #+#             */
/*   Updated: 2018/11/23 22:52:18 by asarandi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

uint32_t	load_command_segment(t_bin *b)
{
	if (b->is_64bit == 1)
		return (LC_SEGMENT_64);
	else
		return (LC_SEGMENT);
}

/*
** this is for getting the vmaddr of a segment
** which will be added to value in nlist for a symbol
*/

void		*get_segment_by_sect_number(t_bin *b, uint8_t sect)
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

void		*get_section_by_number(t_bin *b, uint8_t sect)
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
