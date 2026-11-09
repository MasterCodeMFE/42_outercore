#include "ft_ls.h"

int	get_size_width(t_ls_entry *lista)
{
	int			max;
	int			len;
	long long	size;

	max = 1;
	while (lista != NULL)
	{
		size = (long long)lista->metadata.st_size;
		len = 1;
		while (size >= 10)
		{
			size = size / 10;
			len++;
		}
		if (len > max)
			max = len;
		lista = lista->siguiente;
	}
	return (max);
}
