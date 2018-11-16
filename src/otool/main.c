#include "libft.h"
#include "libftprintf.h"

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
