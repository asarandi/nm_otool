/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sizeof.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asarandi <asarandi@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/18 10:03:28 by asarandi          #+#    #+#             */
/*   Updated: 2018/11/18 11:38:31 by asarandi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "common.h"

uint32_t	sizeof_nlist(t_machof *f)
{
	if (f->is_64bit == 1)
		return (sizeof(struct nlist_64));
	else
		return (sizeof(struct nlist));
}

uint32_t	sizeof_segment(t_machof *f)
{
	if (f->is_64bit == 1)
		return (sizeof(struct segment_command_64));
	else
		return (sizeof(struct segment_command));

}

uint32_t	sizeof_section(t_machof *f)
{
	if (f->is_64bit == 1)
		return (sizeof(struct section_64));
	else
		return (sizeof(struct section));
}

uint32_t	nsects_in_segment(t_machof *f, void *segment)
{
	if (f->is_64bit == 1)
		return (swap32(f, ((struct segment_command_64 *) segment)->nsects));
	else
		return (swap32(f, ((struct segment_command *) segment)->nsects));
}

uint32_t	sizeof_mach_header(t_machof *f)
{
	if (f->is_64bit == 1)
		return (sizeof(struct mach_header_64));
	else
		return (sizeof(struct mach_header));
}
