#include "ft_ls.h"

void	free_entries(t_ls_list *lista)
{
	t_ls_entry	*actual;
	t_ls_entry	*siguiente;

	actual = lista->primero;
	while (actual != NULL)
	{
		siguiente = actual->siguiente;
		free(actual->nombre);
		free(actual->ruta);
		free(actual);
		actual = siguiente;
	}
	lista->primero = NULL;
	lista->ultimo = NULL;
}
