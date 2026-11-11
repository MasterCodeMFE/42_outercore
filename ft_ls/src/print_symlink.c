#include "ft_ls.h"

/**
 * @brief Prints the target of a symbolic link.
 *
 * @details Reads the target path stored in a symbolic link using readlink().
 *
 * Since readlink() does not append a null terminator to the returned string,
 * the number of bytes read is used to manually terminate the buffer before
 * printing it.
 *
 * The symbolic link target is displayed using the same format as ls:
 *
 * link -> target
 *
 * If readlink() fails, the function returns without printing the target.
 *
 * @param path Path of the symbolic link to read.
 */
void	print_symlink(char *path)
{
	char	buffer[4096];
	ssize_t	len;

	len = readlink(path, buffer, sizeof(buffer) - 1);
	if (len == -1)
		return ;
	buffer[len] = '\0';
	printf(" -> %s", buffer);
}
