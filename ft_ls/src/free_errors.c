#include "ft_ls.h"

/**
 * @brief Frees all errors stored in an error list.
 *
 * @details Iterates through the linked error list and releases all
 * dynamically allocated memory associated with each error.
 *
 * For every error node, the function frees:
 *
 * - The path associated with the error.
 * - The error node itself.
 *
 * After all nodes have been released, the first and last pointers of the
 * list are reset to NULL, leaving the error list in an empty state.
 *
 * @param list Error list containing the nodes to free.
 */
void	free_errors(t_ls_error_list *list)
{
	t_ls_error	*current;
	t_ls_error	*next;

	current = list->first;
	while (current != NULL)
	{
		next = current->next;
		free(current->path);
		free(current);
		current = next;
	}
	list->first = NULL;
	list->last = NULL;
}
