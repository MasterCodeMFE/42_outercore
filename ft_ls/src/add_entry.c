#include "ft_ls.h"

int	add_entry(struct dirent *entrada, t_ls_list *lista, char *directorio)
{
	t_ls_entry	*nodo;

	nodo = malloc(sizeof(t_ls_entry));
	if (nodo == NULL)
		return (1);
	nodo->nombre = strdup(entrada->d_name);
	if (nodo->nombre == NULL)
	{
		free(nodo);
		return (1);
	}
	nodo->ruta = join_path(directorio, entrada->d_name);
	if (nodo->ruta == NULL)
	{
		free(nodo->nombre);
		free(nodo);
		return (1);
	}
	if (lstat(nodo->ruta, &nodo->metadata) == -1)
	{
		free(nodo->ruta);
		free(nodo->nombre);
		free(nodo);
		return (1);
	}
	append_entry(lista, nodo);
	return (0);
}
