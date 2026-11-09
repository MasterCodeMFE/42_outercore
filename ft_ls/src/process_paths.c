#include "ft_ls.h"

int	process_paths(int argc, char **argv, t_ls_flags options)
{
	int				error;
	t_ls_error_list	errores;
	t_ls_list		archivos;
	t_ls_list		directorios;

	errores.primero = NULL;
	errores.ultimo = NULL;
	archivos.primero = NULL;
	archivos.ultimo = NULL;
	directorios.primero = NULL;
	directorios.ultimo = NULL;
	error = 0;
	if (optind == argc)
	{
		if (read_directory(".", options) == 1)
			error = 1;
	}
	else
	{
		if (classify_paths(argc, argv, &archivos, &directorios, &errores, &error) == 1)
			error = 1;
		else
		{
			print_errors(errores.primero);
			if (process_files(&archivos, options) == 1)
				error = 1;
			if (archivos.primero != NULL
				&& directorios.primero != NULL)
				printf("\n");
			if (process_directories(&directorios,
					&archivos, options) == 1)
				error = 1;
		}
	}
	free_errors(&errores);
	free_entries(&archivos);
	free_entries(&directorios);
	return (error);
}
