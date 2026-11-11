#include "ft_ls.h"

/**
 * @brief Creates and appends a new error to the error list.
 *
 * @details Allocates a new t_ls_error node and stores the path associated
 * with the error and its corresponding error number.
 *
 * The function performs the following operations:
 *
 * - Allocates memory for a new t_ls_error node.
 * - Copies the path associated with the error.
 * - Stores the corresponding error number.
 * - Initializes the next pointer to NULL.
 * - Appends the error to the end of the provided error list.
 *
 * If the list is empty, the new error becomes both the first and last
 * element of the list.
 *
 * If a memory allocation fails, all previously allocated resources are
 * released before returning an error.
 *
 * @param list      Error list where the new error will be appended.
 * @param path      Path associated with the error.
 * @param error_num Error number associated with the path.
 *
 * @return 0 on success, or 1 if a memory allocation fails.
 */
int	add_error(t_ls_error_list *list, char *path, int error_num)
{
	t_ls_error	*new_error;

	new_error = malloc(sizeof(t_ls_error));
	if (new_error == NULL)
		return (1);
	new_error->path = strdup(path);
	if (new_error->path == NULL)
	{
		free(new_error);
		return (1);
	}
	new_error->error_num = error_num;
	new_error->next = NULL;
	if (list->first == NULL)
	{
		list->first = new_error;
		list->last = new_error;
		return (0);
	}
	list->last->next = new_error;
	list->last = new_error;
	return (0);
}
