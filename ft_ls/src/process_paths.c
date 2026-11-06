#include "ft_ls.h"

int	process_paths(int argc, char **argv, t_ls_flags options)
{
	int			i;
    int	error;
	struct stat	metadata;

	i = optind;
    error = 0;
	if (i == argc)
	{
		if (read_directory(".", options) == 1)
            error = 1;
	}
    else
    {
        while (i < argc)
        {
            if (lstat(argv[i], &metadata) == -1)
            {
                fprintf(stderr, "ft_ls: cannot access '%s': %s\n", argv[i], strerror(errno));
                error = 1;
            }
            else if (S_ISDIR(metadata.st_mode))
            {
                if (options.R == 0 && argc - optind > 1)
                    printf("%s:\n", argv[i]);
                if (read_directory(argv[i], options) == 1)
                    error = 1;
            }
            else
                printf("%s\n", argv[i]);
            i++;
            if (i < argc)
                printf("\n");
        }
    }
	return (error);
}