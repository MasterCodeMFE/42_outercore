#include "ft_ls.h"

/**
 * @brief Prints the size information of a file entry.
 *
 * @details Prints the value displayed in the size column of the long
 * format of ls.
 *
 * For regular files, directories, symbolic links, FIFOs, and sockets,
 * the file size stored in st_size is printed and aligned using the
 * provided column width.
 *
 * For character and block devices, the device identifier stored in
 * st_rdev is printed instead of the file size:
 *
 * - major() identifies the device driver or device class.
 * - minor() identifies the specific device managed by that driver.
 *
 * The major and minor fields are right-aligned independently using their
 * respective maximum widths. This keeps the comma and both number columns
 * aligned across a listing.
 *
 * @param entry  Entry containing the file metadata.
 * @param widths Structure containing the column widths used for alignment.
 */
void	print_size(t_ls_entry *entry, t_ls_widths widths)
{
	if (S_ISCHR(entry->metadata.st_mode) || S_ISBLK(entry->metadata.st_mode))
	{
		printf("%*u, %*u ", widths.device_major, major(entry->metadata.st_rdev), widths.device_minor, minor(entry->metadata.st_rdev));
	}
	else
	{
		printf("%*lld ", widths.size, (long long)entry->metadata.st_size);
	}
}
