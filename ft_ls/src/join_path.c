#include "ft_ls.h"

char *join_path(char *directorio, char *nombre)
{
	char	*ruta;
	size_t	len;

	len = strlen(directorio) + strlen(nombre) + 2;
	ruta = malloc(sizeof(char) * len);
	if (ruta == NULL)
		return (NULL);
	strcpy(ruta, directorio);
	if (ruta[strlen(ruta) - 1] != '/')
    {
		strcat(ruta, "/");
    }
    strcat(ruta, nombre);
	return (ruta);
}