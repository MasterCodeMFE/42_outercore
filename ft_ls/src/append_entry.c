#include "ft_ls.h"

/**
 * @brief Appends an entry node to the end of a list.
 *
 * @details Initializes the node's next pointer and appends the node to the
 * end of the provided list.
 *
 * If the list is empty, the new node becomes both the first and last
 * element. Otherwise, the current last node is linked to the new node,
 * and the last pointer is updated.
 *
 * @param list List where the node will be appended.
 * @param node Entry node to append.
 */
void	append_entry(t_ls_list *list, t_ls_entry *node)
{
	node->next = NULL;
	if (list->first == NULL)
	{
		list->first = node;
		list->last = node;
	}
	else
	{
		list->last->next = node;
		list->last = node;
	}
}