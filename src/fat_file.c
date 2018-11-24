/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fat_file.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asarandi <asarandi@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/23 19:33:14 by asarandi          #+#    #+#             */
/*   Updated: 2018/11/23 22:48:55 by asarandi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "common.h"

uint32_t	fat_file_find_host_arch(t_file *f)
{
	uint32_t	i;
	void		*fat_arch;

	i = 0;
	while (i < file_nfat_arch(f))
	{
		fat_arch = &f->map[
			sizeof(struct fat_header) + (sizeof_fat_arch(f) * i)];
		if ((fat_arch_cputype(f, fat_arch) == HOST_CPU_TYPE) &&
			(fat_arch_cpusubtype(f, fat_arch) == HOST_CPU_SUBTYPE))
			return (i);
		i++;
	}
	return (0xffffffff);
}

int			fat_file_process(t_file *f, void *fat_arch, void (*func)(t_bin *))
{
	t_bin	b;

	(void)ft_memset(&b, 0, sizeof(t_bin));
	b.data = &f->map[fat_arch_offset(f, fat_arch)];
	b.fsize = fat_arch_size(f, fat_arch);
	b.parent = f;
	(void)fat_file_cpu_info(f, &b, fat_arch);
	process_macho(&b, func);
	return (0);
}

int			fat_file_loader(t_file *f, void (*func)(t_bin *))
{
	uint32_t	i;
	void		*fat_arch;

	i = fat_file_find_host_arch(f);
	if (i != 0xffffffff)
	{
		fat_arch = &f->map[sizeof(struct fat_header) +
			(sizeof_fat_arch(f) * i)];
		return (fat_file_process(f, fat_arch, func));
	}
	else
	{
		i = 0;
		while (i < file_nfat_arch(f))
		{
			fat_arch = &f->map[sizeof(struct fat_header) +
				(sizeof_fat_arch(f) * i)];
			(void)fat_file_process(f, fat_arch, func);
			i++;
		}
	}
	return (0);
}
