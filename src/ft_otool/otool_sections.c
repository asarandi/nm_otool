/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   otool_sections.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asarandi <asarandi@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/23 21:43:39 by asarandi          #+#    #+#             */
/*   Updated: 2018/11/23 22:48:18 by asarandi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "otool.h"

uint64_t	section_addr(t_bin *b, void *sect)
{
	if (b->is_64bit == 1)
		return (swap64(b, ((struct section_64 *)sect)->addr));
	else
		return (swap32(b, ((struct section *)sect)->addr));
}

uint64_t	section_size(t_bin *b, void *sect)
{
	if (b->is_64bit == 1)
		return (swap64(b, ((struct section_64 *)sect)->size));
	else
		return (swap32(b, ((struct section *)sect)->size));
}

uint32_t	section_offset(t_bin *b, void *sect)
{
	if (b->is_64bit == 1)
		return (swap32(b, ((struct section_64 *)sect)->offset));
	else
		return (swap32(b, ((struct section *)sect)->offset));
}
