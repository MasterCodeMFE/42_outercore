#include "ft_ls.h"

int	classify_paths(int argc, char **argv, t_ls_list *archivos, t_ls_list *directorios, t_ls_error_list *errores, int *error)
{
	int			i;
	struct stat	metadata;

	i = optind;
	while (i < argc)
	{
		if (lstat(argv[i], &metadata) == -1)
		{
			if (add_error(errores, argv[i], errno) == 1)
				return (1);
			*error = 1;
		}
		else if (S_ISDIR(metadata.st_mode))
		{
			if (add_path(directorios, argv[i], metadata) == 1)
				return (1);
		}
		else
		{
			if (add_path(archivos, argv[i], metadata) == 1)
				return (1);
		}
		i++;
	}
	return (0);
}
