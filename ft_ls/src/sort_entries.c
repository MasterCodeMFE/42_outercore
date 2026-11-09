#include "ft_ls.h"

static void	swap_entries(t_ls_entry *entry)
{
	char		*nombre;
	char		*ruta;
	struct stat	metadata;

	nombre = entry->nombre;
	ruta = entry->ruta;
	metadata = entry->metadata;

	entry->nombre = entry->siguiente->nombre;
	entry->ruta = entry->siguiente->ruta;
	entry->metadata = entry->siguiente->metadata;

	entry->siguiente->nombre = nombre;
	entry->siguiente->ruta = ruta;
	entry->siguiente->metadata = metadata;
}

void	sort_entries(t_ls_list *lista)
{
    t_ls_entry *entry;

    entry = lista->primero;
    while(entry != NULL)
    {
        if (entry->siguiente != NULL && strcmp(entry->nombre, entry->siguiente->nombre) > 0)
        {
            swap_entries(entry);
            entry = lista->primero;
            continue;
        }
        entry = entry->siguiente;
    }
}

void	reverse_sort_entries(t_ls_list *lista)
{
    t_ls_entry *entry;

    entry = lista->primero;
    while(entry != NULL)
    {
        if (entry->siguiente != NULL && strcmp(entry->nombre, entry->siguiente->nombre) < 0)
        {
            swap_entries(entry);
            entry = lista->primero;
            continue;
        }
        entry = entry->siguiente;
    }
}

static int	compare_time(t_ls_entry *a, t_ls_entry *b)
{
	if (a->metadata.st_mtim.tv_sec > b->metadata.st_mtim.tv_sec)
		return (-1);
	if (a->metadata.st_mtim.tv_sec < b->metadata.st_mtim.tv_sec)
		return (1);
	if (a->metadata.st_mtim.tv_nsec > b->metadata.st_mtim.tv_nsec)
		return (-1);
	if (a->metadata.st_mtim.tv_nsec < b->metadata.st_mtim.tv_nsec)
		return (1);
	return (strcmp(a->nombre, b->nombre));
}

void	time_sort_entries(t_ls_list *lista)
{
    t_ls_entry *entry;

    entry = lista->primero;
    while(entry != NULL)
    {
        if (entry->siguiente != NULL && compare_time(entry, entry->siguiente) > 0)
        {
            swap_entries(entry);
            entry = lista->primero;
            continue;
        }
        entry = entry->siguiente;
    }
}

void	reverse_time_sort_entries(t_ls_list *lista)
{
    t_ls_entry *entry;

    entry = lista->primero;
    while(entry != NULL)
    {
        if (entry->siguiente != NULL && compare_time(entry, entry->siguiente) < 0)
        {
            swap_entries(entry);
            entry = lista->primero;
            continue;
        }
        entry = entry->siguiente;
    }
}
