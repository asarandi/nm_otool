/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asarandi <asarandi@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/16 01:02:57 by asarandi          #+#    #+#             */
/*   Updated: 2018/11/18 03:26:26 by asarandi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "otool.h"

int main(int ac, char **av)
{
	int	i;

	i = 1;
	while (i < ac)
	{
		ft_printf("process file: %s\n", av[i]);
		i++;
	}
	if (i == 1)
		(void)show_usage(av[1]);
	return (0);
}
