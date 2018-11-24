/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   otool_print.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asarandi <asarandi@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/23 21:46:35 by asarandi          #+#    #+#             */
/*   Updated: 2018/11/23 22:48:21 by asarandi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "otool.h"
#define OTOOL_ROW_LENGTH 16

int	otool_print_byte(unsigned char c)
{
	const char	hex[] = "0123456789abcdef";
	char		out[3];

	out[0] = hex[c / 16];
	out[1] = hex[c % 16];
	out[2] = ' ';
	write(1, &out[0], 3);
	return (0);
}

int	otool_print_address(t_bin *b, uint64_t x)
{
	if (b->is_64bit == 1)
		ft_printf("\n%016lx\t", x);
	else
		ft_printf("\n%08x\t", x);
	return (0);
}

int	otool_print_section(t_bin *b, void *seg, void *sect)
{
	uint64_t		i;
	uint64_t		size;
	uint64_t		addr;
	uint32_t		offset;
	unsigned char	c;

	(void)ft_printf("%s:\nContents of (%s,%s) section", b->fn,
			((struct segment_command *)seg)->segname,
			((struct section *)sect)->sectname);
	i = 0;
	offset = section_offset(b, sect);
	size = section_size(b, sect);
	addr = section_addr(b, sect);
	while (i < size)
	{
		if (i % OTOOL_ROW_LENGTH == 0)
			(void)otool_print_address(b, addr + i);
		c = ((unsigned char *)b->data)[offset + i];
		(void)otool_print_byte(c);
		i++;
	}
	(void)ft_printf("\n");
	return (0);
}
