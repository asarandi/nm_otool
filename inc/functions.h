/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   functions.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asarandi <asarandi@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/23 19:46:47 by asarandi          #+#    #+#             */
/*   Updated: 2018/11/23 20:51:46 by asarandi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FUNCTIONS_H
# define FUNCTIONS_H

#include "common.h"

typedef struct s_file			t_file;
typedef struct s_bin			t_bin;
typedef struct load_command		t_lc;
typedef struct symtab_command	t_stc;
typedef struct nlist			t_nlist;


char		*ft_strcpx(char *dst, char *src, size_t n);
char		*get_symname_from_nlist(t_bin *b, t_stc *stc, t_nlist *sym);
char		get_symchar_from_ntype(t_bin *b, t_nlist *sym);
char		get_symchar_from_section(t_bin *b, uint8_t n_sect);
int			fat_file_loader(t_file *f, void(*func)(t_bin *));
int			fat_file_process(t_file *f, void *fat_arch, void(*func)(t_bin *));
int			is_print_symaddr(char c);
int			archive_file_loader(t_file *f, void(*func)(t_bin *));
int			compare_symbols(t_bin *b, t_stc *stc, t_nlist *sym1, t_nlist *sym2);
int			fclose_msgerr(int fd, char *s1, char *s2);
int			is_archive_file(t_file *f);
int			is_fat_file(t_file *f);
int			is_macho_file(t_file *f);
int			is_symdef(char *s);
int			is_valid_magic(void *single, int *is_64bit, int *is_swapped);
int			msgerr(char *err, char *fn);
int			print_arch_info(t_file *f, void *fat_arch);
int			process_macho(t_bin *b, void (*func)(t_bin *));
int			process_file(t_file *f, void (*func)(t_bin *));
int			binary_loader(t_file *f, void (*func)(t_bin *));
int			show_usage(char *s1);
int			validate_macho(t_bin *b);
t_lc		*get_lcmd_by_index(t_bin *b, uint32_t cmd, uint32_t idx);
t_nlist		**sort_symptr_array(t_bin *b, t_stc *stc, t_nlist **array, uint32_t n);
t_nlist		**populate_symptr_array(t_bin *b, t_stc *stc, t_nlist **array);
uint16_t	swap16(t_bin *b, uint16_t x);
uint16_t	swap16u(uint16_t x);
uint32_t	count_symbols_to_display(t_bin *b, t_stc *stc);
uint32_t	fat_arch_cpusubtype(t_file *f, void *fat_arch);
uint32_t	fat_arch_cputype(t_file *f, void *fat_arch);
uint32_t	fat_file_find_host_arch(t_file *f);
uint32_t	file_nfat_arch(t_file *f);
uint32_t	load_command_segment(t_bin *b);
uint32_t	nsects_in_segment(t_bin *b, void *segment);
uint32_t	sizeof_fat_arch(t_file *f);
uint32_t	sizeof_mach_header(t_bin *b);
uint32_t	sizeof_nlist(t_bin *b);
uint32_t	sizeof_section(t_bin *b);
uint32_t	sizeof_segment(t_bin *b);
uint32_t	swap32(t_bin *b, uint32_t x);
uint32_t	swap32u(uint32_t x);
uint64_t	fat_arch_offset(t_file *f, void *header);
uint64_t	fat_arch_size(t_file *f, void *header);
uint64_t	get_symaddr_from_nlist(t_bin *b, t_nlist *sym);
uint64_t	nlist_n_value(t_bin *b, t_nlist *nlist);
uint64_t	segment_fileoff(t_bin *b, void *seg);
uint64_t	segment_vmaddr(t_bin *b, void *seg);
uint64_t	swap64(t_bin *b, uint64_t x);
uint64_t	swap64u(uint64_t x);
void		*get_section_by_name_idx(t_bin *b, void *seg, char *sn, uint32_t i);
void		*get_section_by_number(t_bin *b, uint8_t sect);
void		*get_segment_by_name_idx(t_bin *b, char *segname, uint32_t idx);
void		*get_segment_by_sect_number(t_bin *b, uint8_t sect);
void		nm_print(t_bin *b, t_stc *stc);
void		print_parent_name(t_bin *b);
void		print_symbol(t_bin *b, uint64_t addr, char sym, char *name);
void		print_symptr_array(t_bin *b, t_stc *stc, t_nlist **array, uint32_t n);
void		fat_file_cpu_info(t_file *f, t_bin *b, void *fat_arch);
void		fat_print_arches(t_file *f);
void		run_nm(t_bin *b);
void		run_otool(t_bin *b);

#endif
