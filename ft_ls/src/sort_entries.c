#include "ft_ls.h"

void	sort_entries(t_ls_list *lista)
{
    char *nombre;
    char *ruta;
    struct stat metadata;
    t_ls_entry *entry;

    entry = lista->primero;
    while(entry != NULL)
    {
        if (entry->siguiente != NULL && strcmp(entry->nombre, entry->siguiente->nombre) > 0)
        {
            nombre = entry->nombre;
            ruta = entry->ruta;
            metadata = entry->metadata;
            entry->nombre = entry->siguiente->nombre;
            entry->ruta = entry->siguiente->ruta;
            entry->metadata = entry->siguiente->metadata;
            entry->siguiente->nombre = nombre;
            entry->siguiente->ruta = ruta;
            entry->siguiente->metadata = metadata;
            entry = lista->primero;
            continue;
        }
        entry = entry->siguiente;
    }
}

void	reverse_sort_entries(t_ls_list *lista)
{
    char *nombre;
    char *ruta;
    struct stat metadata;
    t_ls_entry *entry;

    entry = lista->primero;
    while(entry != NULL)
    {
        if (entry->siguiente != NULL && strcmp(entry->nombre, entry->siguiente->nombre) < 0)
        {
            nombre = entry->nombre;
            ruta = entry->ruta;
            metadata = entry->metadata;
            entry->nombre = entry->siguiente->nombre;
            entry->ruta = entry->siguiente->ruta;
            entry->metadata = entry->siguiente->metadata;
            entry->siguiente->nombre = nombre;
            entry->siguiente->ruta = ruta;
            entry->siguiente->metadata = metadata;
            entry = lista->primero;
            continue;
        }
        entry = entry->siguiente;
    }
}

void	time_sort_entries(t_ls_list *lista)
{
    char *nombre;
    char *ruta;
    struct stat metadata;
    t_ls_entry *entry;

    entry = lista->primero;
    while(entry != NULL)
    {
        if (entry->siguiente != NULL && entry->metadata.st_mtime < entry->siguiente->metadata.st_mtime)
        {
            nombre = entry->nombre;
            ruta = entry->ruta;
            metadata = entry->metadata;
            entry->nombre = entry->siguiente->nombre;
            entry->ruta = entry->siguiente->ruta;
            entry->metadata = entry->siguiente->metadata;
            entry->siguiente->nombre = nombre;
            entry->siguiente->ruta = ruta;
            entry->siguiente->metadata = metadata;
            entry = lista->primero;
            continue;
        }
        entry = entry->siguiente;
    }
}

void	reverse_time_sort_entries(t_ls_list *lista)
{
    char *nombre;
    char *ruta;
    struct stat metadata;
    t_ls_entry *entry;

    entry = lista->primero;
    while(entry != NULL)
    {
        if (entry->siguiente != NULL && entry->metadata.st_mtime > entry->siguiente->metadata.st_mtime)
        {
            nombre = entry->nombre;
            ruta = entry->ruta;
            metadata = entry->metadata;
            entry->nombre = entry->siguiente->nombre;
            entry->ruta = entry->siguiente->ruta;
            entry->metadata = entry->siguiente->metadata;
            entry->siguiente->nombre = nombre;
            entry->siguiente->ruta = ruta;
            entry->siguiente->metadata = metadata;
            entry = lista->primero;
            continue;
        }
        entry = entry->siguiente;
    }
}