/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   otool_segments.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asarandi <asarandi@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/23 22:37:31 by asarandi          #+#    #+#             */
/*   Updated: 2018/11/23 22:48:16 by asarandi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "otool.h"

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
