/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fat_cpu.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asarandi <asarandi@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/23 19:33:42 by asarandi          #+#    #+#             */
/*   Updated: 2018/11/23 22:48:57 by asarandi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "common.h"

uint32_t	fat_arch_cputype(t_file *f, void *fat_arch)
{
	if (f->is_swapped)
		return (swap32u(((struct fat_arch *)fat_arch)->cputype));
	else
		return (((struct fat_arch *)fat_arch)->cputype);
}

uint32_t	fat_arch_cpusubtype(t_file *f, void *fat_arch)
{
	if (f->is_swapped)
		return (swap32u(((struct fat_arch *)fat_arch)->cpusubtype));
	else
		return (((struct fat_arch *)fat_arch)->cpusubtype);
}

void		fat_file_cpu_info(t_file *f, t_bin *b, void *fatarch)
{
	uint32_t		cputype;
	uint32_t		cpusubtype;
	struct fat_arch	*fat_arch;

	fat_arch = (struct fat_arch *)fatarch;
	cputype = fat_arch_cputype(f, fat_arch);
	cpusubtype = fat_arch_cpusubtype(f, fat_arch);
	if ((cputype == 0x00000007) && (cpusubtype == 0x00000003))
		b->arch = ARCH_I386;
	else if ((cputype == 0x01000007) && (cpusubtype == 0x80000003))
		b->arch = ARCH_X86_64;
	else if ((cputype == 0x0000000c) && (cpusubtype == 0x00000009))
		b->arch = ARCH_ARMV7;
	else if ((cputype == 0x0000000c) && (cpusubtype == 0x0000000b))
		b->arch = ARCH_ARMV7S;
	else if ((cputype == 0x0100000c) && (cpusubtype == 0x00000000))
		b->arch = ARCH_ARM64;
	else
		b->arch = EMPTY_STRING;
	return ;
}
