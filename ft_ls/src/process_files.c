#include "ft_ls.h"

int	process_files(t_ls_list *archivos, t_ls_flags options)
{
	int			error;
	int			size_width;
	t_ls_entry	*actual;

	error = 0;
	size_width = 1;
	sort_list(archivos, options);
	if (options.l == 1)
		size_width = get_size_width(archivos->primero);
	actual = archivos->primero;
	while (actual != NULL)
	{
		if (read_file(actual, options, size_width) == 1)
			error = 1;
		actual = actual->siguiente;
	}
	if (archivos->primero != NULL && options.l == 0)
		printf("\n");
	return (error);
}
