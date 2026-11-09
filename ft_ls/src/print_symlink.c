#include "ft_ls.h"

void	print_symlink(char *ruta)
{
	char	buffer[4096];
	ssize_t	len;

	len = readlink(ruta, buffer, sizeof(buffer) - 1);
	if (len == -1)
		return;
	buffer[len] = '\0';
	printf(" -> %s", buffer);
}
