#include "ft_ls.h"

void	print_errors(t_ls_error *error)
{
	while (error != NULL)
	{
		fprintf(stderr, "ft_ls: cannot access '%s': %s\n",
			error->ruta, strerror(error->error_num));
		error = error->siguiente;
	}
}
