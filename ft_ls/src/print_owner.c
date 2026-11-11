#include "ft_ls.h"

/**
 * @brief Prints the owner associated with a file.
 *
 * @details Resolves the user name associated with the provided UID using
 * getpwuid().
 *
 * If the user can be resolved, the user name is printed left-aligned using
 * the provided column width. Otherwise, the numeric UID is printed instead.
 *
 * @param uid   User identifier associated with the file.
 * @param width Width used to align the owner column.
 */
void	print_owner(uid_t uid, int width)
{
	struct passwd	*user;

	user = getpwuid(uid);
	if (user != NULL)
		printf("%-*s ", width, user->pw_name);
	else
		printf("%-*u ", width, (unsigned int)uid);
}
