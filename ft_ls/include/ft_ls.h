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
# include <errno.h>
# include <pwd.h>
# include <grp.h>
# include <time.h>

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

    typedef struct s_ls_error
    {
        char				*ruta;
        int					error_num;
        struct s_ls_error	*siguiente;
    }	t_ls_error;

    typedef struct s_ls_error_list
    {
        t_ls_error	*primero;
        t_ls_error	*ultimo;
    }	t_ls_error_list;

    int read_directory(char *directorio, t_ls_flags options);
    int add_entry(struct dirent *entrada, t_ls_list *lista, char *directorio);
    char *join_path(char *directorio, char *nombre);
    void sort_entries(t_ls_list *lista);
    int	parse_flags(int argc, char **argv, t_ls_flags *flags);
    void reverse_sort_entries(t_ls_list *lista);
    int	process_paths(int argc, char **argv, t_ls_flags options);
    void	free_entries(t_ls_list *lista);
    void	time_sort_entries(t_ls_list *lista);
    void	reverse_time_sort_entries(t_ls_list *lista);
    int	add_error(t_ls_error_list *lista, char *ruta, int error_num);
    void	print_errors(t_ls_error *error);
    void	free_errors(t_ls_error_list *lista);
    int	add_path(t_ls_list *lista, char *ruta, struct stat metadata);
    int read_file(t_ls_entry *archivo, t_ls_flags options, int size_width);
    void	print_permissions(mode_t mode);
    void	print_owner(uid_t uid);
    void	print_group(uid_t gid);
    void	print_date(time_t mtime);
    int	get_size_width(t_ls_entry *lista);
    
    
#endif
