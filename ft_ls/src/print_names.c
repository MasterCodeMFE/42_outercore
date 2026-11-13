#include "ft_ls.h"

/**
 * @brief Prints all visible entries from a list.
 *
 * @details Iterates through the linked list and prints every entry that
 * should be visible according to the active options.
 *
 * When the -l option is enabled:
 *
 * - The required column widths are calculated before printing.
 * - Each entry is printed using long-format output.
 *
 * When the -l option is disabled, each visible entry is printed on its own
 * line. This keeps output deterministic when redirected and avoids terminal
 * column-layout behavior.
 *
 * Hidden entries are ignored unless the -a option is enabled.
 *
 * @param list    List of entries to print.
 * @param options Active ft_ls options.
 * @param widths  Column widths calculated for the list being printed.
 */
void	print_names(t_ls_entry *list, t_ls_flags options, t_ls_widths widths)
{
	while (list != NULL)
	{
		if (!(list->name[0] == '.' && options.a == 0))
		{
			print_entry(list, options, widths);
			if (options.l == 0)
				printf("\n");
		}
		list = list->next;
	}
}
