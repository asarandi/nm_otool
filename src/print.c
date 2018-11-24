/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asarandi <asarandi@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/23 19:41:25 by asarandi          #+#    #+#             */
/*   Updated: 2018/11/23 19:43:23 by asarandi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "common.h"

void	print_symbol(t_bin *b, uint64_t addr, char sym, char *name)
{
	if (b->is_64bit)
	{
		if (is_print_symaddr(sym) == 1)
			ft_printf("%016llx %c %s", addr, sym, name);
		else
			ft_printf("%16s %c %s", "", sym, name);
	}
	else
	{
		if (is_print_symaddr(sym) == 1)
			ft_printf("%08llx %c %s", addr & 0xffffffff, sym, name);
		else
			ft_printf("%8s %c %s", "", sym, name);
	}
	if (sym == 'I')
		ft_printf(" (indirect for %s)", name);
	ft_printf("\n");
	return ;
}


void	print_symptr_array(t_bin *b, t_stc *stc, t_nlist **array, uint32_t n)
{
	uint32_t	i;
	char		*symname;
	uint64_t	symaddr;
	char		symchar;

	i = 0;
	while (i < n)
	{
		symname = get_symname_from_nlist(b, stc, array[i]);
		symchar = get_symchar_from_ntype(b, array[i]);
		symaddr = get_symaddr_from_nlist(b, array[i]);
		print_symbol(b, symaddr, symchar, symname);
		i++;
	}
	return ;
}

/*
** if (ft_strcmp  ... is a reproduction of a bug in nm
*/

void	print_parent_name(t_bin *b)
{
	if (b->parent->is_archive)
		ft_printf("\n%s(%s):\n", b->parent->fn, b->fn);
	else if (b->parent->is_fat)
	{
		if (ft_strcmp(b->arch, HOST_ARCH) != 0)
		{
			ft_printf("\n%s (for architecture %s):\n",
				b->parent->fn, b->arch);
		}
	}
	else if (b->parent->print_names)
		ft_printf("\n%s:\n", b->parent->fn);
	return ;
}

void	nm_print(t_bin *b, t_stc *stc)
{
	uint32_t		count;
	struct nlist	**array;

	count = count_symbols_to_display(b, stc);
	array = ft_memalloc((count + 1) * sizeof_nlist(b));
	array = populate_symptr_array(b, stc, array);
	array = sort_symptr_array(b, stc, array, count);
	(void)print_parent_name(b);
	(void)print_symptr_array(b, stc, array, count);
	free(array);
	return ;
}
