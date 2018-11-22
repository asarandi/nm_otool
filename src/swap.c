/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   swap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asarandi <asarandi@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/18 02:47:27 by asarandi          #+#    #+#             */
/*   Updated: 2018/11/21 20:22:10 by asarandi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "common.h"

uint16_t	swap16(t_bin *b, uint16_t x)
{
	uint16_t	res;

	res = 0;
	if (b->is_swapped)
	{
		res += (x >> 0x08) & 0x00ff;
		res += (x << 0x08) & 0xff00;
	}
	else
		res = x;
	return (res);
}

uint32_t	swap32(t_bin *b, uint32_t x)
{
	uint32_t	res;

	res = 0;
	if (b->is_swapped)
	{
		res += (x >> 0x18) & 0x000000ff;
		res += (x >> 0x08) & 0x0000ff00;
		res += (x << 0x08) & 0x00ff0000;
		res += (x << 0x18) & 0xff000000;
	}
	else
		res = x;
	return (res);
}

uint64_t	swap64(t_bin *b, uint64_t x)
{
	uint64_t	res;

	res = 0;
	if (b->is_swapped)
	{
		res += (x >> 0x38) & 0x00000000000000ff;
		res += (x >> 0x28) & 0x000000000000ff00;
		res += (x >> 0x18) & 0x0000000000ff0000;
		res += (x >> 0x08) & 0x00000000ff000000;
		res += (x << 0x08) & 0x000000ff00000000;
		res += (x << 0x18) & 0x0000ff0000000000;
		res += (x << 0x28) & 0x00ff000000000000;
		res += (x << 0x38) & 0xff00000000000000;
	}
	else
		res = x;
	return (res);
}
