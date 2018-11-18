/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   messages.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asarandi <asarandi@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/18 02:49:51 by asarandi          #+#    #+#             */
/*   Updated: 2018/11/18 04:38:18 by asarandi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "common.h"

int	show_usage(char *s1)
{
	ft_printf("usage: %s file\n", s1);
	return (0);
}

int	msgerr(char *err, char *fn)
{
	ft_fprintf(STDERR_FILENO,
			"{red}error{eoc} [{yellow}%s{eoc}]: %s\n", err, fn);
	return (1);
}

int	fclose_msgerr(int fd, char *s1, char *s2)
{
	(void)close(fd);
	return (msgerr(s1, s2));
}
