#ifndef FT_LS_H
#define FT_LS_H

# include <sys/types.h>
# include <sys/stat.h>
# include <dirent.h>
# include <stddef.h>
# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <unistd.h>

    typedef  struct  s_ls_entry
    {
        char *nombre;
        char *ruta;
        struct stat metadata;
        struct s_ls_entry  *siguiente;
    } t_ls_entry;

    typedef struct s_ls_list
    {
        t_ls_entry	*primero;
        t_ls_entry	*ultimo;
    }	t_ls_list;
    
    typedef struct s_ls_flags
    {
        int	l;
        int	R;
        int	a;
        int	r;
        int	t;
    }	t_ls_flags;

    int read_directory(char *directorio, t_ls_flags options);
    int add_entry(struct dirent *entrada, t_ls_list *lista, char *directorio);
    char *join_path(char *directorio, char *nombre);
    void sort_entries(t_ls_list *lista);
    int	parse_flags(int argc, char **argv, t_ls_flags *flags);
    void reverse_sort_entries(t_ls_list *lista);
    int	process_paths(int argc, char **argv, t_ls_flags options);
    
#endif
