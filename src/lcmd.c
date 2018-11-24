/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lcmd.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asarandi <asarandi@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/18 10:11:31 by asarandi          #+#    #+#             */
/*   Updated: 2018/11/23 22:48:33 by asarandi         ###   ########.fr       */
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
