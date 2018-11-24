/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   archive.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asarandi <asarandi@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/23 19:37:54 by asarandi          #+#    #+#             */
/*   Updated: 2018/11/24 03:11:12 by asarandi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "common.h"

/*
** copy n bytes from src to dst, and null terminate dest
*/

char	*ft_strcpx(char *dst, char *src, size_t n)
{
	size_t i;

	i = 0;
	while (i < n)
	{
		if (!src[i])
			break ;
		dst[i] = src[i];
		i++;
	}
	dst[i] = 0;
	return (dst);
}

/*
** man 5 ar
*/

void	archive_child_values(t_bin *b, char *buf, void *arhdr)
{
	struct ar_hdr	*ah;

	ah = (struct ar_hdr *)arhdr;
	b->fsize = ft_atoi(ft_strcpx(buf, ah->ar_size, sizeof(ah->ar_size)));
	b->data = arhdr + sizeof(struct ar_hdr);
	if (ft_strncmp(ah->ar_name, AR_EFMT1, ft_strlen(AR_EFMT1)) == 0)
	{
		(void)ft_strcpx(buf, (char *)ah + sizeof(struct ar_hdr),
				ft_atoi(ah->ar_name + ft_strlen(AR_EFMT1)));
		b->data += ft_atoi(ah->ar_name + ft_strlen(AR_EFMT1));
		b->fsize -= ft_atoi(ah->ar_name + ft_strlen(AR_EFMT1));
	}
	else
		(void)ft_strcpx(buf, ah->ar_name, sizeof(ah->ar_name));
	b->fn = buf;
	return ;
}

int		archive_file_loader(t_file *f, void (*func)(t_bin *))
{
	struct ar_hdr	*ah;
	static char		buf[4096];
	t_bin			b;

	ah = (struct ar_hdr *)&f->map[SARMAG];
	while ((void *)ah - f->map < f->st.st_size)
	{
		(void)ft_memset(&b, 0, sizeof(t_bin));
		if (ft_strncmp(ah->ar_fmag, ARFMAG, ft_strlen(ARFMAG)) != 0)
			return (msgerr(E_ARBADFMAG, f->fn));
		b.parent = f;
		(void)archive_child_values(&b, &buf[0], ah);
		if (!is_symdef(b.fn))
			process_macho(&b, func);
		ah = (struct ar_hdr *)&b.data[b.fsize];
	}
	return (0);
}
