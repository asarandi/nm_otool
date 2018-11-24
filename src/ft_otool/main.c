/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asarandi <asarandi@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/16 01:02:57 by asarandi          #+#    #+#             */
/*   Updated: 2018/11/24 03:03:34 by asarandi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "otool.h"

int		otool_print_section(t_bin *b, void *sect)
{
	uint64_t		i;
	uint64_t		size;
	uint64_t		addr;
	uint32_t		offset;

	(void)otool_print_fn(b);
	(void)ft_printf("Contents of (%s,%s) section",
			((struct section *)sect)->segname,
			((struct section *)sect)->sectname);
	i = 0;
	offset = section_offset(b, sect);
	size = section_size(b, sect);
	addr = section_addr(b, sect);
	while (i < size)
	{
		if (i % OTOOL_ROW_LENGTH == 0)
			(void)otool_print_address(b, addr + i);
		(void)otool_print_style(b, &b->data[offset + i], &i, size);
	}
	(void)ft_printf("\n");
	return (0);
}

int		otool_find(t_bin *b, char *s1, char *s2)
{
	void		*seg;
	void		*sec;
	uint32_t	i;
	uint32_t	j;

	if (validate_macho(b) != 0)
		return (1);
	if (swap32(b, b->mh->filetype) == MH_OBJECT)
		s1 = EMPTY_STRING;
	i = 0;
	while ((seg = get_segment_by_name_idx(b, s1, i++)) != NULL)
	{
		j = 0;
		while ((sec = get_section_by_name_idx(b, seg, s2, j++)) != NULL)
		{
			(void)otool_print_section(b, sec);
		}
	}
	return (0);
}

void	run_otool(t_bin *b)
{
	return ((void)otool_find(b, "__TEXT", "__text"));
}

int		main(int ac, char **av)
{
	int		i;
	t_file	f;

	i = 1;
	while (i < ac)
	{
		(void)ft_memset(&f, 0, sizeof(t_file));
		if (ac > 2)
			f.print_names = 1;
		f.fn = av[i];
		(void)process_file(&f, &run_otool);
		i++;
	}
	if (i == 1)
	{
		(void)ft_memset(&f, 0, sizeof(t_file));
		f.fn = "a.out";
		(void)process_file(&f, &run_otool);
	}
	return (0);
}
