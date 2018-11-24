/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   swapx.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asarandi <asarandi@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/23 19:13:08 by asarandi          #+#    #+#             */
/*   Updated: 2018/11/23 22:48:51 by asarandi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "common.h"

uint16_t	swap16u(uint16_t x)
{
	uint16_t	res;

	res = 0;
	res += (x >> 0x08) & 0x00ff;
	res += (x << 0x08) & 0xff00;
	return (res);
}

uint32_t	swap32u(uint32_t x)
{
	uint32_t	res;

	res = 0;
	res += (x >> 0x18) & 0x000000ff;
	res += (x >> 0x08) & 0x0000ff00;
	res += (x << 0x08) & 0x00ff0000;
	res += (x << 0x18) & 0xff000000;
	return (res);
}

uint64_t	swap64u(uint64_t x)
{
	uint64_t	res;

	res = 0;
	res += (x >> 0x38) & 0x00000000000000ff;
	res += (x >> 0x28) & 0x000000000000ff00;
	res += (x >> 0x18) & 0x0000000000ff0000;
	res += (x >> 0x08) & 0x00000000ff000000;
	res += (x << 0x08) & 0x000000ff00000000;
	res += (x << 0x18) & 0x0000ff0000000000;
	res += (x << 0x28) & 0x00ff000000000000;
	res += (x << 0x38) & 0xff00000000000000;
	return (res);
}
