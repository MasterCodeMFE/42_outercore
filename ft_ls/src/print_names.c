#include "ft_ls.h"

/**
 * @brief Checks whether another visible entry exists after the current one.
 *
 * @details Iterates through the remaining entries in the linked list and
 * determines whether at least one entry should still be displayed.
 *
 * Hidden entries are ignored unless the -a option is enabled. This check
 * is used to determine whether another separator or the final newline must
 * be printed when long-format output is disabled.
 *
 * @param entry   Current entry in the linked list.
 * @param options Active ft_ls options.
 *
 * @return 1 if another visible entry exists, otherwise 0.
 */
static int	has_visible_after(t_ls_entry *entry, t_ls_flags options)
{
	entry = entry->next;
	while (entry != NULL)
	{
		if (!(entry->name[0] == '.' && options.a == 0))
			return (1);
		entry = entry->next;
	}
	return (0);
}

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
 * When the -l option is disabled:
 *
 * - Entries are separated by two spaces.
 * - The final visible entry is followed by a newline.
 *
 * Hidden entries are ignored unless the -a option is enabled.
 *
 * @param list    List of entries to print.
 * @param options Active ft_ls options.
 */
void	print_names(t_ls_entry *list, t_ls_flags options)
{
	t_ls_widths	widths;

	widths.links = 1;
	widths.owner = 1;
	widths.group = 1;
	widths.size = 1;
	if (options.l == 1)
		widths = get_widths(list, options);
	while (list != NULL)
	{
		if (!(list->name[0] == '.' && options.a == 0))
		{
			print_entry(list, options, widths);
			if (options.l == 0)
			{
				if (has_visible_after(list, options))
					printf("  ");
				else
					printf("\n");
			}
		}
		list = list->next;
	}
}
