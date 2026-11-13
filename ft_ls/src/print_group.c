#include "ft_ls.h"

/**
 * @brief Looks up a group by GID while reusing the most recent lookup.
 *
 * @details Directory listings commonly contain many entries in the same
 * group. The one-entry cache avoids repeating the NSS lookup for consecutive
 * entries with that GID. The returned pointer is used immediately and must
 * not be retained across a lookup for another GID.
 *
 * @param gid Group identifier to look up.
 *
 * @return The matching group record, or NULL if the GID is unknown.
 */
struct group	*lookup_group_by_gid(gid_t gid)
{
	static gid_t			last_gid;
	static int				has_cache;
	static struct group	*cached_group;

	if (has_cache == 0 || last_gid != gid)
	{
		cached_group = getgrgid(gid);
		last_gid = gid;
		has_cache = 1;
	}
	return (cached_group);
}

/**
 * @brief Prints the group associated with a file.
 *
 * @details Resolves the group name associated with the provided GID using
 * getgrgid().
 *
 * If the group can be resolved, the group name is printed left-aligned
 * using the provided column width. Otherwise, the numeric GID is printed
 * instead.
 *
 * @param gid   Group identifier associated with the file.
 * @param width Width used to align the group column.
 */
void	print_group(gid_t gid, int width)
{
	struct group	*group;

	group = lookup_group_by_gid(gid);
	if (group != NULL)
		printf("%-*s ", width, group->gr_name);
	else
		printf("%-*u ", width, (unsigned int)gid);
}
