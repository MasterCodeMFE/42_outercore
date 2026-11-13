#include "ft_ls.h"

/**
 * @brief Prints all errors stored in the error list.
 *
 * @details Iterates through the linked list of errors and prints a
 * descriptive message to the standard error output for each inaccessible
 * path.
 *
 * Each error message includes:
 *
 * - The path that caused the error.
 * - The system error message associated with the stored error number.
 *
 * strerror() is used to convert the stored errno value into a
 * human-readable error message.
 *
 * @param error First error node in the linked error list.
 */
void	print_errors(t_ls_error *error)
{
	while (error != NULL)
	{
		fprintf(stderr, "ft_ls: cannot access '%s': %s\n", error->path, strerror(error->error_num));
		error = error->next;
	}
}
