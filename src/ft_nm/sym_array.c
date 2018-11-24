/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sym_array.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asarandi <asarandi@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/23 19:26:47 by asarandi          #+#    #+#             */
/*   Updated: 2018/11/23 23:55:43 by asarandi         ###   ########.fr       */
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
