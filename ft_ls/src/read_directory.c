#include "ft_ls.h"

void	print_names(t_ls_entry *lista, t_ls_flags options)
{
	while (lista != NULL)
	{
		if (!(lista->nombre[0] == '.' && options.a == 0))
		{
			printf("%s", lista->nombre);
			if (lista->siguiente != NULL)
				printf("  ");
			else
				printf("\n");
		}
		lista = lista->siguiente;
	}
}

int read_directory(char *directorio, t_ls_flags options)
{
    DIR				*opendirectory;
    struct dirent	*entrada;
    t_ls_entry		*actual;
    t_ls_list		lista;

    lista.primero = NULL;
    lista.ultimo = NULL;
    opendirectory = opendir(directorio);
    if(opendirectory == NULL)
    {
        return(1);
    }
    entrada = readdir(opendirectory);
    while(entrada != NULL)
    {
        if(add_entry(entrada, &lista, directorio) == 1)
        {
			closedir(opendirectory);
			free_entries(&lista);
            return (1);
        }
        entrada = readdir(opendirectory);
    }
	closedir(opendirectory);
	if (options.t == 0)
	{
		if (options.r == 0)
			sort_entries(&lista);
		else
			reverse_sort_entries(&lista);
	}
	else
	{
		if (options.r == 0)
			time_sort_entries(&lista);
		else
			reverse_time_sort_entries(&lista);
	}
	if(options.R == 1)
	{
		printf("%s:\n", directorio);
	}
    print_names(lista.primero, options);
	if(options.R == 1)
	{
		actual = lista.primero;
		while (actual != NULL)
		{
			if(S_ISDIR(actual->metadata.st_mode) && strcmp(actual->nombre, ".") != 0 && strcmp(actual->nombre, "..") != 0)
			{
				printf("\n");
				read_directory(actual->ruta, options);
			}
			actual = actual->siguiente;
		}
	}
	free_entries(&lista);
    return (0);
}
