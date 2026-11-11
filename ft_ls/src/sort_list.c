#include "ft_ls.h"

/**
 * @brief Sorts an entry list according to the active sorting options.
 *
 * @details Selects the appropriate sorting method based on the -t and -r
 * options:
 *
 * - No -t and no -r: Sorts alphabetically in ascending order.
 * - -r only: Sorts alphabetically in descending order.
 * - -t only: Sorts by modification time, newest first.
 * - -t and -r: Sorts by modification time, oldest first.
 *
 * The actual sorting operation is delegated to the corresponding sorting
 * function.
 *
 * @param list    List containing the entries to sort.
 * @param options Active ft_ls options.
 */
void	sort_list(t_ls_list *list, t_ls_flags options)
{
	if (options.t == 0)
	{
		if (options.r == 0)
			sort_entries(list);
		else
			reverse_sort_entries(list);
	}
	else
	{
		if (options.r == 0)
			time_sort_entries(list);
		else
			reverse_time_sort_entries(list);
	}
}
