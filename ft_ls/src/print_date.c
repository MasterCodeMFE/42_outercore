#include "ft_ls.h"

/**
 * @brief Prints the modification date of a file in ls long format.
 *
 * @details Formats the modification timestamp using the same date style
 * used by ls for recent, old, and future files.
 *
 * The displayed format depends on the difference between the current time
 * and the file modification time:
 *
 * - Recent files display the month, day, hour, and minute.
 * - Files older than approximately six months display the year.
 * - Files more than one hour in the future also display the year.
 *
 * The resulting formats are:
 *
 * - Recent file: "Nov 11 10:18"
 * - Old or future file: "Jan 10  2020"
 *
 * If localtime() fails, the function returns without printing a date.
 *
 * @param mtime File modification timestamp.
 */
void	print_date(time_t mtime)
{
	struct tm	*time_info;
	char		buffer[64];
	time_t		now;
	double		diff;

	now = time(NULL);
	diff = difftime(now, mtime);
	time_info = localtime(&mtime);
	if (time_info == NULL)
		return ;
	if (diff > 15778476 || diff < -3600)
		strftime(buffer, sizeof(buffer), "%b %e  %Y", time_info);
	else
		strftime(buffer, sizeof(buffer), "%b %e %H:%M", time_info);
	printf("%s ", buffer);
}
