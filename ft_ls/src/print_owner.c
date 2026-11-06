#include "ft_ls.h"

void	print_owner(uid_t uid)
{
	struct passwd	*user;

	user = getpwuid(uid);
	if (user != NULL)
		printf("%s ", user->pw_name);
	else
		printf("%u ", (unsigned int)uid);
}