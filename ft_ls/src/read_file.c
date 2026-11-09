#include "ft_ls.h"

int	read_file(t_ls_entry *archivo, t_ls_flags options, int size_width)
{
	print_entry(archivo, options, size_width);
	if (options.l == 0)
		printf("  ");
	return (0);
}
