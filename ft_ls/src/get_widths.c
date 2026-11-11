#include "ft_ls.h"

/**
 * @brief Returns the number of digits required to print a number.
 *
 * @param number Number whose display width will be calculated.
 *
 * @return Number of digits required to represent the number.
 */
static int	number_width(long long number)
{
	int	len;

	len = 1;
	while (number >= 10)
	{
		number = number / 10;
		len++;
	}
	return (len);
}

/**
 * @brief Updates the maximum owner column width.
 *
 * @details Resolves the user name associated with the given UID using
 * getpwuid(). If the user cannot be resolved, the numeric UID is used
 * to calculate the required column width.
 *
 * @param widths Structure containing the current column widths.
 * @param uid    User identifier to evaluate.
 */
static void	update_owner_width(t_ls_widths *widths, uid_t uid)
{
	struct passwd	*user;
	int				len;

	user = getpwuid(uid);
	if (user != NULL)
		len = strlen(user->pw_name);
	else
		len = number_width((long long)uid);
	if (len > widths->owner)
		widths->owner = len;
}

/**
 * @brief Updates the maximum group column width.
 *
 * @details Resolves the group name associated with the given GID using
 * getgrgid(). If the group cannot be resolved, the numeric GID is used
 * to calculate the required column width.
 *
 * @param widths Structure containing the current column widths.
 * @param gid    Group identifier to evaluate.
 */
static void	update_group_width(t_ls_widths *widths, gid_t gid)
{
	struct group	*group;
	int				len;

	group = getgrgid(gid);
	if (group != NULL)
		len = strlen(group->gr_name);
	else
		len = number_width((long long)gid);
	if (len > widths->group)
		widths->group = len;
}

/**
 * @brief Updates the maximum numeric column widths for an entry.
 *
 * @details Calculates the display width required for the link count and
 * file size of the provided entry. The stored maximum values are updated
 * when the current entry requires a wider column.
 *
 * @param widths Structure containing the current column widths.
 * @param entry  Entry whose numeric metadata will be evaluated.
 */
static void	update_number_widths(t_ls_widths *widths, t_ls_entry *entry)
{
	int	len;

	len = number_width((long long)entry->metadata.st_nlink);
	if (len > widths->links)
		widths->links = len;
	len = number_width((long long)entry->metadata.st_size);
	if (len > widths->size)
		widths->size = len;
}

/**
 * @brief Calculates the column widths required for long-format output.
 *
 * @details Iterates through the entry list and calculates the maximum
 * width required for the following columns:
 *
 * - Number of hard links.
 * - Owner name or numeric UID.
 * - Group name or numeric GID.
 * - File size.
 *
 * Hidden entries are excluded from the calculation unless the -a option
 * is enabled.
 *
 * @param list    List of entries used to calculate the column widths.
 * @param options Active ft_ls options.
 *
 * @return Structure containing the calculated column widths.
 */
t_ls_widths	get_widths(t_ls_entry *list, t_ls_flags options)
{
	t_ls_widths	widths;

	widths.links = 1;
	widths.owner = 1;
	widths.group = 1;
	widths.size = 1;
	while (list != NULL)
	{
		if (!(list->name[0] == '.' && options.a == 0))
		{
			update_number_widths(&widths, list);
			update_owner_width(&widths, list->metadata.st_uid);
			update_group_width(&widths, list->metadata.st_gid);
		}
		list = list->next;
	}
	return (widths);
}
