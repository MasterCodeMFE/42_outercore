#include "ft_ls.h"

/**
 * @brief Prints the character representing the file type.
 *
 * @details Determines the file type from the mode bits and prints the
 * corresponding character used by the long format of ls.
 *
 * Supported file types:
 *
 * - '-' Regular file.
 * - 'd' Directory.
 * - 'l' Symbolic link.
 * - 'c' Character device.
 * - 'b' Block device.
 * - 'p' FIFO (named pipe).
 * - 's' Socket.
 * - '?' Unknown or unsupported file type.
 *
 * @param mode File mode containing the file type information.
 */
void	print_file_type(mode_t mode)
{
	if (S_ISREG(mode))
		printf("-");
	else if (S_ISDIR(mode))
		printf("d");
	else if (S_ISLNK(mode))
		printf("l");
	else if (S_ISCHR(mode))
		printf("c");
	else if (S_ISBLK(mode))
		printf("b");
	else if (S_ISFIFO(mode))
		printf("p");
	else if (S_ISSOCK(mode))
		printf("s");
	else
		printf("?");
}
