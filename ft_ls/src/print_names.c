#include "ft_ls.h"

void	print_names(t_ls_entry *lista, t_ls_flags options)
{
	int	size_width;

	size_width = 1;
	if (options.l == 1)
		size_width = get_size_width(lista);

	while (lista != NULL)
	{
		if (!(lista->nombre[0] == '.' && options.a == 0))
		{
			print_entry(lista, options, size_width);
			if (options.l == 0)
			{
				if (lista->siguiente != NULL)
					printf("  ");
				else
					printf("\n");
			}
		}
		lista = lista->siguiente;
	}
}
