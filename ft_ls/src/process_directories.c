#include "ft_ls.h"

int	process_directories(t_ls_list *directorios, t_ls_list *archivos, t_ls_flags options)
{
	int			error;
	t_ls_entry	*actual;

	error = 0;
	sort_list(directorios, options);
	actual = directorios->primero;
	while (actual != NULL)
	{
		if ((directorios->primero != directorios->ultimo || archivos->primero != NULL) && options.R == 0)
			printf("%s:\n", actual->ruta);
		if (read_directory(actual->ruta, options) == 1)
			error = 1;
		actual = actual->siguiente;
		if (actual != NULL)
			printf("\n");
	}
	return (error);
}
