/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   otool_print.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asarandi <asarandi@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/23 21:46:35 by asarandi          #+#    #+#             */
/*   Updated: 2018/11/24 02:51:27 by asarandi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "otool.h"

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

int	otool_print_dword(uint32_t ui)
{
	const char		hex[] = "0123456789abcdef";
	char			out[9];
	unsigned char	c;

	c = (ui >> 24) & 0xff;
	out[0] = hex[c / 16];
	out[1] = hex[c % 16];
	c = (ui >> 16) & 0xff;
	out[2] = hex[c / 16];
	out[3] = hex[c % 16];
	c = (ui >> 8) & 0xff;
	out[4] = hex[c / 16];
	out[5] = hex[c % 16];
	c = ui & 0xff;
	out[6] = hex[c / 16];
	out[7] = hex[c % 16];
	out[8] = ' ';
	write(1, &out[0], 9);
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

int	otool_print_style(t_bin *b, void *data, uint64_t *idx, uint64_t size)
{
	uint32_t	cputype;

	cputype = swap32(b, b->mh->cputype);
	if (((cputype & 0xff) != CPU_TYPE_X86) && (*idx <= size - 4))
	{
		(void)otool_print_dword(
				swap32(b, *(uint32_t *)data));
		(*idx) += 4;
	}
	else
	{
		(void)otool_print_byte(*(unsigned char *)data);
		(*idx)++;
	}
	return (0);
}

int	otool_print_fn(t_bin *b)
{
	if (b->parent->is_archive)
		ft_printf("%s(%s):\n", b->parent->fn, b->fn);
	else if (b->parent->is_fat)
	{
		ft_printf("%s", b->parent->fn);
		if ((ft_strcmp(b->arch, HOST_ARCH) != 0) &&
				(b->arch != EMPTY_STRING))
			ft_printf(" (architecture %s)", b->arch);
		ft_printf(":\n");
	}
	else if (b->parent->print_names)
		ft_printf("%s:\n", b->parent->fn);
	return (0);
}
