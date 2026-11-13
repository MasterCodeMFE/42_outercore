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

	user = lookup_user_by_uid(uid);
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

	group = lookup_group_by_gid(gid);
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
 * @brief Updates the widths for a character or block device.
 *
 * @details Calculates the digit count of the device's major and minor
 * numbers separately. Keeping separate maximum widths aligns both numeric
 * fields across the directory listing.
 *
 * @param widths Structure containing the current column widths.
 * @param entry  Device entry whose major and minor numbers are evaluated.
 */
static void	update_device_widths(t_ls_widths *widths, t_ls_entry *entry)
{
	int	len;

	len = number_width((long long)major(entry->metadata.st_rdev));
	if (len > widths->device_major)
		widths->device_major = len;
	len = number_width((long long)minor(entry->metadata.st_rdev));
	if (len > widths->device_minor)
		widths->device_minor = len;
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
 * - Major and minor device numbers, calculated separately.
 *
 * Hidden entries are excluded from the calculation unless the -a option
 * is enabled. Entries without valid stat metadata do not contribute to
 * column widths.
 *
 * @param list    List of entries used to calculate the column widths.
 * @param options Active ft_ls options.
 *
 * @return Structure containing the calculated column widths.
 */
t_ls_widths	get_widths(t_ls_entry *list, t_ls_flags options)
{
	t_ls_widths	widths;
	int			has_device;

	widths.links = 1;
	widths.owner = 1;
	widths.group = 1;
	widths.size = 1;
	widths.device_major = 1;
	widths.device_minor = 1;
	has_device = 0;
	while (list != NULL)
	{
		if (list->metadata_valid == 1
			&& !(list->name[0] == '.' && options.a == 0))
		{
			update_number_widths(&widths, list);
			if (S_ISCHR(list->metadata.st_mode) || S_ISBLK(list->metadata.st_mode))
			{
				update_device_widths(&widths, list);
				has_device = 1;
			}
			update_owner_width(&widths, list->metadata.st_uid);
			update_group_width(&widths, list->metadata.st_gid);
		}
		list = list->next;
	}
	if (has_device && widths.device_major + widths.device_minor + 2
		> widths.size)
		widths.size = widths.device_major + widths.device_minor + 2;
	return (widths);
}
