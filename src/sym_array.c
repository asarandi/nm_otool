/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sym_array.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asarandi <asarandi@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/23 19:26:47 by asarandi          #+#    #+#             */
/*   Updated: 2018/11/23 21:18:52 by asarandi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "common.h"

t_nlist	**populate_symptr_array(t_bin *b, t_stc *stc, t_nlist **array)
{
	uint32_t		i;
	uint32_t		j;
	uint32_t		symoff;
	struct nlist	*nlist;

	symoff = swap32(b, stc->symoff);
	i = 0;
	j = 0;
	while (i < swap32(b, stc->nsyms))
	{
		nlist = (struct nlist *)&b->data[symoff + i * sizeof_nlist(b)];
		if (!(nlist->n_type & N_STAB))
		{
			array[j++] = nlist;
		}
		i++;
	}
	array[j] = NULL;
	return (array);
}

t_u32	count_symbols_to_display(t_bin *b, t_stc *stc)
{
	uint32_t		i;
	uint32_t		count;
	uint32_t		symoff;
	struct nlist	*nlist;

	symoff = swap32(b, stc->symoff);
	i = 0;
	count = 0;
	while (i < swap32(b, stc->nsyms))
	{
		nlist = (struct nlist *)&b->data[symoff + i * sizeof_nlist(b)];
		if (!(nlist->n_type & N_STAB))
			count++;
		i++;
	}
	return (count);
}

int		compare_symbols(t_bin *b, t_stc *stc, t_nlist *sym1, t_nlist *sym2)
{
	uint32_t	stroff;
	uint32_t	n_strx;
	char		*sym1_name;
	char		*sym2_name;
	int			res;

	stroff = swap32(b, stc->stroff);
	n_strx = swap32(b, sym1->n_un.n_strx);
	sym1_name = (char *)&b->data[stroff + n_strx];
	n_strx = swap32(b, sym2->n_un.n_strx);
	sym2_name = (char *)&b->data[stroff + n_strx];
	res = ft_strcmp(sym1_name, sym2_name);
	if (!res)
		res = (int)(nlist_n_value(b, sym1) - nlist_n_value(b, sym2));
	return (res);
}

t_nlist	**sort_symptr_array(t_bin *b, t_stc *stc, t_nlist **array, uint32_t n)
{
	uint32_t	i;
	int			swapped;
	t_nlist		*tmp;

	while (1)
	{
		swapped = 0;
		i = 1;
		while (i <= n - 1)
		{
			if (compare_symbols(b, stc, array[i - 1], array[i]) > 0)
			{
				tmp = array[i - 1];
				array[i - 1] = array[i];
				array[i] = tmp;
				swapped = 1;
			}
			i++;
		}
		if (!swapped)
			break ;
	}
	return (array);
}
