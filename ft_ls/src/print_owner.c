#include "ft_ls.h"

/**
 * @brief Looks up a user by UID while reusing the most recent lookup.
 *
 * @details Directory listings commonly contain many entries owned by the
 * same user. The one-entry cache avoids repeating the NSS lookup for
 * consecutive entries with that UID. The returned pointer is used
 * immediately and must not be retained across a lookup for another UID.
 *
 * @param uid User identifier to look up.
 *
 * @return The matching passwd record, or NULL if the UID is unknown.
 */
struct passwd	*lookup_user_by_uid(uid_t uid)
{
	static uid_t		last_uid;
	static int			has_cache;
	static struct passwd	*cached_user;

	if (has_cache == 0 || last_uid != uid)
	{
		cached_user = getpwuid(uid);
		last_uid = uid;
		has_cache = 1;
	}
	return (cached_user);
}

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

	user = lookup_user_by_uid(uid);
	if (user != NULL)
		printf("%-*s ", width, user->pw_name);
	else
		printf("%-*u ", width, (unsigned int)uid);
}
