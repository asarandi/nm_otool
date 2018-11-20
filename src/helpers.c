/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asarandi <asarandi@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/19 17:56:13 by asarandi          #+#    #+#             */
/*   Updated: 2018/11/19 17:59:21 by asarandi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "common.h"

uint64_t	nlist_n_value(t_machof *f, t_nlist *nlist)
{
	uint64_t	value;

	if (f->is_64bit)
		value = swap64(f, nlist->n_value);
	else
		value = swap32(f, nlist->n_value);
	return (value);
}

uint64_t	segment_vmaddr(t_machof *f, void *seg)
{
	uint64_t	res;

	if (f->is_64bit)
		res = swap64(f, ((struct segment_command_64 *)seg)->vmaddr);
	else
		res = swap32(f, ((struct segment_command *) seg)->vmaddr);
	return (res);
}

uint64_t	segment_fileoff(t_machof *f, void *seg)
{
	uint64_t	res;

	if (f->is_64bit)
		res = swap64(f, ((struct segment_command_64 *)seg)->fileoff);
	else
		res = swap32(f, ((struct segment_command *) seg)->fileoff);
	return (res);
}
