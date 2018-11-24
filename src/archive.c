/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   archive.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asarandi <asarandi@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/23 19:37:54 by asarandi          #+#    #+#             */
/*   Updated: 2018/11/23 20:48:23 by asarandi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "common.h"

/*
** man 5 ar
*/
int	archive_file_loader(t_file *f, void(*func)(t_bin *))
{
	struct ar_hdr	*ah;
	static char	buf[4096];
	t_bin	b;

	ah = (struct ar_hdr *)&f->map[SARMAG];
	while ((void *)ah - f->map < f->st.st_size)
	{
		(void)ft_memset(&b, 0, sizeof(t_bin));
		if (ft_strncmp(ah->ar_fmag, ARFMAG, ft_strlen(ARFMAG)) != 0)
			return (msgerr(E_ARBADFMAG, f->fn));
			/* XXX implement boundary check */
		b.parent = f;
		b.fsize = ft_atoi(ft_strcpx(buf, ah->ar_size, sizeof(ah->ar_size)));
		b.data = (void *)ah + sizeof(struct ar_hdr);
		if (ft_strncmp(ah->ar_name, AR_EFMT1, ft_strlen(AR_EFMT1)) == 0)
		{
			(void)ft_strcpx(buf, (char *)ah + sizeof(struct ar_hdr), ft_atoi(ah->ar_name + ft_strlen(AR_EFMT1)));
			b.data += ft_atoi(ah->ar_name + ft_strlen(AR_EFMT1));
			b.fsize -= ft_atoi(ah->ar_name + ft_strlen(AR_EFMT1));
		}
		else
			(void)ft_strcpx(buf, ah->ar_name, sizeof(ah->ar_name));
		//buf now has a copy of file name in archive
		b.fn = &buf[0];
//		b.data += (void *)ah + sizeof(struct ar_hdr);
		if (!is_symdef(b.fn))
			process_macho(&b, func);
		ah = (struct ar_hdr *)&b.data[b.fsize];
	}
	return (0);
}
