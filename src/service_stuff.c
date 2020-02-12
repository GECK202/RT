#include "rt.h"

void		ft_exit(char *line)
{
	ft_putstr(line);
	ft_putchar('\n');
	exit (-1);
}