#include "ft_ls.h"

/**
 * @brief Frees all entries stored in a list.
 *
 * @details Iterates through the linked list and releases all dynamically
 * allocated memory associated with each entry.
 *
 * For every node, the function frees:
 *
 * - The entry name.
 * - The complete entry path.
 * - The entry node itself.
 *
 * After all nodes have been released, the first and last pointers of the
 * list are reset to NULL, leaving the list in an empty state.
 *
 * @param list List containing the entries to free.
 */
void	free_entries(t_ls_list *list)
{
	t_ls_entry	*current;
	t_ls_entry	*next;

	current = list->first;
	while (current != NULL)
	{
		next = current->next;
		free(current->name);
		free(current->path);
		free(current);
		current = next;
	}
	list->first = NULL;
	list->last = NULL;
}
