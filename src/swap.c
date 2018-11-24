/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   swap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asarandi <asarandi@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/18 02:47:27 by asarandi          #+#    #+#             */
/*   Updated: 2018/11/23 22:48:49 by asarandi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "common.h"

uint16_t	swap16(t_bin *b, uint16_t x)
{
	if (b->is_swapped)
		return (swap16u(x));
	else
		return (x);
}

uint32_t	swap32(t_bin *b, uint32_t x)
{
	if (b->is_swapped)
		return (swap32u(x));
	else
		return (x);
}

uint64_t	swap64(t_bin *b, uint64_t x)
{
	if (b->is_swapped)
		return (swap64u(x));
	else
		return (x);
}
