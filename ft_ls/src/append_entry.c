#include "ft_ls.h"

void	append_entry(t_ls_list *lista, t_ls_entry *nodo)
{
	nodo->siguiente = NULL;
	if (lista->primero == NULL)
	{
		lista->primero = nodo;
		lista->ultimo = nodo;
	}
	else
	{
		lista->ultimo->siguiente = nodo;
		lista->ultimo = nodo;
	}
}
