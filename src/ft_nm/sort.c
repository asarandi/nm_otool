/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asarandi <asarandi@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/23 23:54:10 by asarandi          #+#    #+#             */
/*   Updated: 2018/11/23 23:56:39 by asarandi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

int			compare_symbols(t_bin *b, t_nlist *sym1, t_nlist *sym2)
{
	uint32_t	stroff;
	uint32_t	n_strx;
	char		*sym1_name;
	char		*sym2_name;
	int			res;

	stroff = swap32(b, b->stc->stroff);
	n_strx = swap32(b, sym1->n_un.n_strx);
	sym1_name = (char *)&b->data[stroff + n_strx];
	n_strx = swap32(b, sym2->n_un.n_strx);
	sym2_name = (char *)&b->data[stroff + n_strx];
	res = ft_strcmp(sym1_name, sym2_name);
	if (!res)
		res = (int)(nlist_n_value(b, sym1) - nlist_n_value(b, sym2));
	return (res);
}

uint32_t	qs_partition(t_bin *b, uint32_t lo, uint32_t hi)
{
	struct nlist	*tmp;
	struct nlist	*pivot;
	uint32_t		i;
	uint32_t		j;

	pivot = b->array[lo];
	i = lo - 1;
	j = hi + 1;
	while (1)
	{
		while (compare_symbols(b, b->array[++i], pivot) < 0)
			continue ;
		while (compare_symbols(b, b->array[--j], pivot) > 0)
			continue ;
		if (i >= j)
			break ;
		tmp = b->array[i];
		b->array[i] = b->array[j];
		b->array[j] = tmp;
	}
	return (j);
}


void		quicksort(t_bin *b, uint32_t lo, uint32_t hi)
{
	uint32_t	p;

	if (lo < hi)
	{
		p = qs_partition(b, lo, hi);
		quicksort(b, lo, p);
		quicksort(b, p + 1, hi);
	}
	return ;
}

t_nlist		**sort_symptr_array(t_bin *b, t_stc *stc, t_nlist **a, uint32_t n)
{
	b->stc = stc;
	b->array = a;
	(void)quicksort(b, 0, n - 1);
	return (a);
}
