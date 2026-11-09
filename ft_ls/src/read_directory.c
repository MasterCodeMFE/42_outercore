#include "ft_ls.h"

static int	read_entries(DIR *opendirectory, t_ls_list *lista,
		char *directorio)
{
	struct dirent	*entrada;

	entrada = readdir(opendirectory);
	while (entrada != NULL)
	{
		if (add_entry(entrada, lista, directorio) == 1)
			return (1);
		entrada = readdir(opendirectory);
	}
	return (0);
}

static int	can_recurse(t_ls_entry *entry, t_ls_flags options)
{
	if (!S_ISDIR(entry->metadata.st_mode))
		return (0);
	if (strcmp(entry->nombre, ".") == 0)
		return (0);
	if (strcmp(entry->nombre, "..") == 0)
		return (0);
	if (options.a == 0 && entry->nombre[0] == '.')
		return (0);
	return (1);
}

static int	recurse_directories(t_ls_list *lista, t_ls_flags options)
{
	int			error;
	t_ls_entry	*actual;

	error = 0;
	actual = lista->primero;
	while (actual != NULL)
	{
		if (can_recurse(actual, options) == 1)
		{
			printf("\n");
			if (read_directory(actual->ruta, options) == 1)
				error = 1;
		}
		actual = actual->siguiente;
	}
	return (error);
}

int	read_directory(char *directorio, t_ls_flags options)
{
	DIR			*opendirectory;
	t_ls_list	lista;
	int			error;

	lista.primero = NULL;
	lista.ultimo = NULL;
	error = 0;
	opendirectory = opendir(directorio);
	if (opendirectory == NULL)
		return (1);
	if (read_entries(opendirectory, &lista, directorio) == 1)
	{
		closedir(opendirectory);
		free_entries(&lista);
		return (1);
	}
	closedir(opendirectory);
	sort_list(&lista, options);
	if (options.R == 1)
		printf("%s:\n", directorio);
	print_names(lista.primero, options);
	if (options.R == 1)
	{
		if (recurse_directories(&lista, options) == 1)
			error = 1;
	}
	free_entries(&lista);
	return (error);
}
