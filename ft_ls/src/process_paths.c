#include "ft_ls.h"

int process_paths(int argc, char **argv, t_ls_flags options)
{
    int             i;
    int             error;
    int             size_width;
    struct stat     metadata;
    t_ls_error_list errores;
    t_ls_list       archivos;
    t_ls_list       directorios;
    t_ls_entry      *actual;

    errores.primero = NULL;
    errores.ultimo = NULL;
    archivos.primero = NULL;
    archivos.ultimo = NULL;
    directorios.primero = NULL;
    directorios.ultimo = NULL;
    i = optind;
    error = 0;
    size_width = 1;

    if (i == argc)
    {
        if (read_directory(".", options) == 1)
            error = 1;
    }
    else
    {
        while (i < argc)
        {
            if (lstat(argv[i], &metadata) == -1)
            {
                if (add_error(&errores, argv[i], errno) == 1)
                {
                    free_errors(&errores);
                    return (1);
                }
                error = 1;
            }
            else if (S_ISDIR(metadata.st_mode))
            {
                if (add_path(&directorios, argv[i], metadata) == 1)
                {
                    free_errors(&errores);
                    free_entries(&archivos);
                    free_entries(&directorios);
                    error = 1;
                    break;
                }
            }
            else
            {
                if (add_path(&archivos, argv[i], metadata) == 1)
                {
                    free_errors(&errores);
                    free_entries(&archivos);
                    free_entries(&directorios);
                    error = 1;
                    break;
                }
            }
            i++;
        }

        print_errors(errores.primero);
        free_errors(&errores);

        if (options.t == 0)
        {
            if (options.r == 0)
                sort_entries(&archivos);
            else
                reverse_sort_entries(&archivos);
        }
        else
        {
            if (options.r == 0)
                time_sort_entries(&archivos);
            else
                reverse_time_sort_entries(&archivos);
        }

        if (options.l == 1)
            size_width = get_size_width(archivos.primero);

        actual = archivos.primero;
        while (actual != NULL)
        {
            if (read_file(actual, options, size_width) == 1)
                error = 1;
            actual = actual->siguiente;
        }

        if (archivos.primero != NULL && options.l == 0)
            printf("\n");

        if (archivos.primero != NULL && directorios.primero != NULL)
            printf("\n");

        if (options.t == 0)
        {
            if (options.r == 0)
                sort_entries(&directorios);
            else
                reverse_sort_entries(&directorios);
        }
        else
        {
            if (options.r == 0)
                time_sort_entries(&directorios);
            else
                reverse_time_sort_entries(&directorios);
        }

        actual = directorios.primero;
        while (actual != NULL)
        {
            if ((directorios.primero != directorios.ultimo
                    || archivos.primero != NULL)
                && options.R == 0)
            {
                printf("%s:\n", actual->ruta);
            }

            if (read_directory(actual->ruta, options) == 1)
                error = 1;

            actual = actual->siguiente;

            if (actual != NULL)
                printf("\n");
        }

        free_entries(&archivos);
        free_entries(&directorios);
    }
    return (error);
}