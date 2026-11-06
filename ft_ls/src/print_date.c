#include "ft_ls.h"

void	print_date(time_t mtime)
{
	struct tm	*time_info;
	char		buffer[64];

	time_info = localtime(&mtime);
	if (time_info == NULL)
		return;
	strftime(buffer, sizeof(buffer), "%b %e %H:%M", time_info);
	printf("%s ", buffer);
}