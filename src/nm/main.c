/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asarandi <asarandi@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/16 01:02:15 by asarandi          #+#    #+#             */
/*   Updated: 2018/11/16 01:02:22 by asarandi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

int	show_usage(void)
{
	ft_printf("%s usage\n", __FILE__);
	return (0);
}

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
		(void)show_usage();
	return (0);
}
