/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asarandi <asarandi@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/16 01:02:15 by asarandi          #+#    #+#             */
/*   Updated: 2018/11/23 21:42:26 by asarandi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

void	run_nm(t_bin *b)
{
	uint32_t				isym;
	struct symtab_command	*stc;
	struct load_command		*lcmd;

	isym = 0;
	while ((lcmd = get_lcmd_by_index(b, LC_SYMTAB, isym++)) != NULL)
	{
		stc = (struct symtab_command *)lcmd;
		b->stc = stc;
		(void)nm_print(b, stc);
	}
	return ;
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
		(void)process_file(&f, &run_nm);
		i++;
	}
	if (i == 1)
	{
		(void)ft_memset(&f, 0, sizeof(t_file));
		f.fn = "a.out";
		(void)process_file(&f, &run_nm);
	}
	return (0);
}
