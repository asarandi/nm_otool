/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   symbol.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asarandi <asarandi@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/23 19:43:54 by asarandi          #+#    #+#             */
/*   Updated: 2018/11/23 21:14:33 by asarandi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "common.h"

char		get_symchar_from_section(t_bin *b, uint8_t n_sect)
{
	void	*sect;
	char	*sectname;
	char	*segname;

	sect = get_section_by_number(b, n_sect);
	if (sect != NULL)
	{
		sectname = ((struct section *)sect)->sectname;
		segname = ((struct section *)sect)->segname;
		if ((ft_strcmp(segname, SEG_TEXT) == 0) &&
				(ft_strcmp(sectname, SECT_TEXT) == 0))
			return ('t');
		if ((ft_strcmp(segname, SEG_DATA) == 0) &&
				(ft_strcmp(sectname, SECT_DATA) == 0))
			return ('d');
		if ((ft_strcmp(segname, SEG_DATA) == 0) &&
				(ft_strcmp(sectname, SECT_BSS) == 0))
			return ('b');
	}
	return ('s');
}

char		get_symchar_from_ntype(t_bin *b, t_nlist *sym)
{
	char	symchar;

	if ((sym->n_type & N_TYPE) == N_UNDF)
		symchar = (nlist_n_value(b, sym) == 0) ? 'u' : 'c';
	else if ((sym->n_type & N_TYPE) == N_PBUD)
		symchar = 'u';
	else if ((sym->n_type & N_TYPE) == N_ABS)
		symchar = 'a';
	else if ((sym->n_type & N_TYPE) == N_SECT)
		symchar = get_symchar_from_section(b, sym->n_sect);
	else if ((sym->n_type & N_TYPE) == N_INDR)
		symchar = 'i';
	else
		symchar = '?';
	if ((sym->n_type & N_EXT) && (symchar != '?'))
		symchar = ft_toupper(symchar);
	return (symchar);
}

char		*get_symname_from_nlist(t_bin *b, t_stc *stc, t_nlist *sym)
{
	uint32_t	n_strx;
	uint32_t	stroff;
	char		*symname;

	stroff = swap32(b, stc->stroff);
	n_strx = swap32(b, sym->n_un.n_strx);
	symname = (char *)&b->data[stroff + n_strx];
	return (symname);
}

uint64_t	get_symaddr_from_nlist(t_bin *b, t_nlist *sym)
{
	uint64_t	symaddr;
	void		*seg;

	symaddr = nlist_n_value(b, sym);
	if (((sym->n_type & N_TYPE) & N_SECT) && (b->is_64bit == 1))
	{
		seg = get_segment_by_sect_number(b, sym->n_sect);
		if (segment_vmaddr(b, seg) != 0)
		{
			symaddr += segment_vmaddr(b, seg);
			symaddr -= segment_fileoff(b, seg);
		}
	}
	return (symaddr);
}
