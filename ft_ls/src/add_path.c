#include "ft_ls.h"

int	add_path(t_ls_list *lista, char *ruta, struct stat metadata)
{
	t_ls_entry	*nodo;

	nodo = malloc(sizeof(t_ls_entry));
	if (nodo == NULL)
		return (1);

	nodo->nombre = strdup(ruta);
	if (nodo->nombre == NULL)
	{
		free(nodo);
		return (1);
	}

	nodo->ruta = strdup(ruta);
	if (nodo->ruta == NULL)
	{
		free(nodo->nombre);
		free(nodo);
		return (1);
	}

	nodo->metadata = metadata;
	nodo->siguiente = NULL;

	if (lista->primero == NULL)
	{
		lista->primero = nodo;
		lista->ultimo = nodo;
		return (0);
	}

	lista->ultimo->siguiente = nodo;
	lista->ultimo = nodo;
	return (0);
}