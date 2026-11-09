#include "ft_ls.h"

void	sort_list(t_ls_list *lista, t_ls_flags options)
{
	if (options.t == 0)
	{
		if (options.r == 0)
			sort_entries(lista);
		else
			reverse_sort_entries(lista);
	}
	else
	{
		if (options.r == 0)
			time_sort_entries(lista);
		else
			reverse_time_sort_entries(lista);
	}
}
