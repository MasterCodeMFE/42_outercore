#include "ft_ls.h"

/**
 * @brief Prints the user permission bits.
 *
 * @details Prints the read, write, and execute permissions for the file
 * owner. The execute position also represents the set-user-ID bit:
 *
 * - 'r' User has read permission.
 * - 'w' User has write permission.
 * - 'x' User has execute permission.
 * - 's' Set-user-ID and execute permission are enabled.
 * - 'S' Set-user-ID is enabled without execute permission.
 * - '-' Corresponding permission is not enabled.
 *
 * @param mode File mode containing the user permission bits.
 */
static void	print_user_permissions(mode_t mode)
{
	if (mode & S_IRUSR)
		printf("r");
	else
		printf("-");
	if (mode & S_IWUSR)
		printf("w");
	else
		printf("-");
	if (mode & S_ISUID)
	{
		if (mode & S_IXUSR)
			printf("s");
		else
			printf("S");
	}
	else if (mode & S_IXUSR)
		printf("x");
	else
		printf("-");
}

/**
 * @brief Prints the group permission bits.
 *
 * @details Prints the read, write, and execute permissions for the file
 * group. The execute position also represents the set-group-ID bit:
 *
 * - 'r' Group has read permission.
 * - 'w' Group has write permission.
 * - 'x' Group has execute permission.
 * - 's' Set-group-ID and execute permission are enabled.
 * - 'S' Set-group-ID is enabled without execute permission.
 * - '-' Corresponding permission is not enabled.
 *
 * @param mode File mode containing the group permission bits.
 */
static void	print_group_permissions(mode_t mode)
{
	if (mode & S_IRGRP)
		printf("r");
	else
		printf("-");
	if (mode & S_IWGRP)
		printf("w");
	else
		printf("-");
	if (mode & S_ISGID)
	{
		if (mode & S_IXGRP)
			printf("s");
		else
			printf("S");
	}
	else if (mode & S_IXGRP)
		printf("x");
	else
		printf("-");
}

/**
 * @brief Prints the permission bits for other users.
 *
 * @details Prints the read, write, and execute permissions for users that
 * are neither the file owner nor members of the file group. The execute
 * position also represents the sticky bit:
 *
 * - 'r' Other users have read permission.
 * - 'w' Other users have write permission.
 * - 'x' Other users have execute permission.
 * - 't' Sticky bit and execute permission are enabled.
 * - 'T' Sticky bit is enabled without execute permission.
 * - '-' Corresponding permission is not enabled.
 *
 * @param mode File mode containing the other permission bits.
 */
static void	print_other_permissions(mode_t mode)
{
	if (mode & S_IROTH)
		printf("r");
	else
		printf("-");
	if (mode & S_IWOTH)
		printf("w");
	else
		printf("-");
	if (mode & S_ISVTX)
	{
		if (mode & S_IXOTH)
			printf("t");
		else
			printf("T");
	}
	else if (mode & S_IXOTH)
		printf("x");
	else
		printf("-");
}

/**
 * @brief Prints the permission bits of a file.
 *
 * @details Prints the nine permission characters used by the long format
 * of ls, divided into user, group, and other permissions.
 *
 * The function also handles the special permission bits:
 *
 * - 's' Set-user-ID or set-group-ID with execute permission.
 * - 'S' Set-user-ID or set-group-ID without execute permission.
 * - 't' Sticky bit with execute permission.
 * - 'T' Sticky bit without execute permission.
 *
 * The resulting permission format is:
 *
 * user  -> rwx
 * group -> rwx
 * other -> rwx
 *
 * @param mode File mode containing the permission and special mode bits.
 */
void	print_permissions(mode_t mode)
{
	print_user_permissions(mode);
	print_group_permissions(mode);
	print_other_permissions(mode);
}
