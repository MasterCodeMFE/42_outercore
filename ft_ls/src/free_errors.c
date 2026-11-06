#include "ft_ls.h"

void	free_errors(t_ls_error_list *lista)
{
	t_ls_error	*actual;
	t_ls_error	*siguiente;

	actual = lista->primero;
	while (actual != NULL)
	{
		siguiente = actual->siguiente;
		free(actual->ruta);
		free(actual);
		actual = siguiente;
	}
	lista->primero = NULL;
	lista->ultimo = NULL;
}