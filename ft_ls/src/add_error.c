#include "ft_ls.h"

int	add_error(t_ls_error_list *lista, char *ruta, int error_num)
{
	t_ls_error	*nuevo;

	nuevo = malloc(sizeof(t_ls_error));
	if (nuevo == NULL)
		return (1);
	nuevo->ruta = strdup(ruta);
	if (nuevo->ruta == NULL)
	{
		free(nuevo);
		return (1);
	}
	nuevo->error_num = error_num;
	nuevo->siguiente = NULL;
	if (lista->primero == NULL)
	{
		lista->primero = nuevo;
		lista->ultimo = nuevo;
		return (0);
	}
	lista->ultimo->siguiente = nuevo;
	lista->ultimo = nuevo;
	return (0);
}