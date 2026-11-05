#define _XOPEN_SOURCE 700
#define _DEFAULT_SOURCE

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#ifdef __linux__
# include <sys/sysmacros.h>
#endif
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <limits.h>
#include <stdint.h>

typedef struct s_flags
{
    int l;
    int R;
    int a;
    int r;
    int t;
}   t_flags;

typedef struct s_entry
{
    char        *name;
    char        *path;
    struct stat info;
}   t_entry;

typedef struct s_list
{
    t_entry *items;
    size_t  count;
    size_t  capacity;
}   t_list;

typedef struct s_widths
{
    int links;
    int owner;
    int group;
    int size;
}   t_widths;

static t_flags g_flags;
static int     g_status = 0;

/* ---------------------------------------------------------- */
/* Errores y memoria                                          */
/* ---------------------------------------------------------- */

static void memory_error(void)
{
    fprintf(stderr, "ft_ls: memory allocation failed\n");
    exit(1);
}

static void *xmalloc(size_t size)
{
    void *ptr;

    ptr = malloc(size);
    if (!ptr)
        memory_error();
    return (ptr);
}

static char *ft_strdup(const char *src)
{
    size_t len;
    char   *copy;

    len = strlen(src);
    copy = xmalloc(len + 1);
    memcpy(copy, src, len + 1);
    return (copy);
}

static void report_error(const char *path)
{
    int saved_errno;

    saved_errno = errno;
    fprintf(stderr, "ft_ls: %s: %s\n",
        path, strerror(saved_errno));
    g_status = 1;
}

/* ---------------------------------------------------------- */
/* Parseo                                                     */
/* ---------------------------------------------------------- */

static int set_flag(char c, t_flags *flags)
{
    if (c == 'l')
        flags->l = 1;
    else if (c == 'R')
        flags->R = 1;
    else if (c == 'a')
        flags->a = 1;
    else if (c == 'r')
        flags->r = 1;
    else if (c == 't')
        flags->t = 1;
    else
        return (0);
    return (1);
}

static int parse_option(const char *arg, t_flags *flags)
{
    size_t i;

    i = 1;
    while (arg[i])
    {
        if (!set_flag(arg[i], flags))
        {
            fprintf(stderr,
                "ft_ls: invalid option -- '%c'\n", arg[i]);
            fprintf(stderr,
                "usage: ft_ls [-Ralrt] [file ...]\n");
            return (0);
        }
        i++;
    }
    return (1);
}

static int parse_args(
    int argc,
    char **argv,
    t_flags *flags,
    char **paths,
    int *path_count)
{
    int i;
    int options;

    i = 1;
    options = 1;
    *path_count = 0;
    while (i < argc)
    {
        if (options && strcmp(argv[i], "--") == 0)
            options = 0;
        else if (options
            && argv[i][0] == '-'
            && argv[i][1] != '\0')
        {
            if (!parse_option(argv[i], flags))
                return (0);
        }
        else
            paths[(*path_count)++] = argv[i];
        i++;
    }
    return (1);
}

/* ---------------------------------------------------------- */
/* Rutas y listas                                             */
/* ---------------------------------------------------------- */

static char *join_path(const char *dir, const char *name)
{
    size_t dl;
    size_t nl;
    size_t slash;
    char   *path;

    dl = strlen(dir);
    nl = strlen(name);
    slash = (dl > 0 && dir[dl - 1] != '/');
    if (dl > SIZE_MAX - nl - slash - 1)
        memory_error();
    path = xmalloc(dl + slash + nl + 1);
    memcpy(path, dir, dl);
    if (slash)
        path[dl++] = '/';
    memcpy(path + dl, name, nl + 1);
    return (path);
}

static void list_init(t_list *list)
{
    list->items = NULL;
    list->count = 0;
    list->capacity = 0;
}

static void list_free(t_list *list)
{
    size_t i;

    i = 0;
    while (i < list->count)
    {
        free(list->items[i].name);
        free(list->items[i].path);
        i++;
    }
    free(list->items);
    list_init(list);
}

static void list_grow(t_list *list)
{
    size_t  capacity;
    t_entry *items;

    if (list->capacity > SIZE_MAX / 2)
        memory_error();
    capacity = list->capacity ? list->capacity * 2 : 16;
    if (capacity > SIZE_MAX / sizeof(*items))
        memory_error();
    items = realloc(list->items, capacity * sizeof(*items));
    if (!items)
        memory_error();
    list->items = items;
    list->capacity = capacity;
}

static void list_push(t_list *list, t_entry *entry)
{
    if (list->count == list->capacity)
        list_grow(list);
    list->items[list->count++] = *entry;
    entry->name = NULL;
    entry->path = NULL;
}

static int make_entry(
    const char *path,
    const char *name,
    t_entry *entry)
{
    if (lstat(path, &entry->info) == -1)
    {
        report_error(path);
        return (0);
    }
    entry->name = ft_strdup(name);
    entry->path = ft_strdup(path);
    return (1);
}

/* ---------------------------------------------------------- */
/* Ordenación                                                 */
/* ---------------------------------------------------------- */

static int compare_names(const t_entry *a, const t_entry *b)
{
    int result;

    result = strcoll(a->name, b->name);
    if (result == 0)
        result = strcmp(a->name, b->name);
    return ((result > 0) - (result < 0));
}

static long entry_nsec(const t_entry *entry)
{
#if defined(__APPLE__)
    return (entry->info.st_mtimespec.tv_nsec);
#else
    return (entry->info.st_mtim.tv_nsec);
#endif
}

static int compare_times(const t_entry *a, const t_entry *b)
{
    if (a->info.st_mtime > b->info.st_mtime)
        return (-1);
    if (a->info.st_mtime < b->info.st_mtime)
        return (1);
    if (entry_nsec(a) > entry_nsec(b))
        return (-1);
    if (entry_nsec(a) < entry_nsec(b))
        return (1);
    return (compare_names(a, b));
}

static int compare_entries(const void *left, const void *right)
{
    const t_entry *a;
    const t_entry *b;
    int           result;

    a = left;
    b = right;
    if (g_flags.t)
        result = compare_times(a, b);
    else
        result = compare_names(a, b);
    if (g_flags.r)
        result = -result;
    return (result);
}

static void sort_list(t_list *list, const t_flags *flags)
{
    g_flags = *flags;
    if (list->count > 1)
    {
        qsort(list->items, list->count,
            sizeof(*list->items), compare_entries);
    }
}

/* ---------------------------------------------------------- */
/* Lectura de directorios                                     */
/* ---------------------------------------------------------- */

static int visible_name(const char *name, const t_flags *flags)
{
    return (flags->a || name[0] != '.');
}

static int read_directory(
    const char *directory,
    const t_flags *flags,
    t_list *list)
{
    DIR           *dir;
    struct dirent *item;
    t_entry       entry;
    char          *path;

    dir = opendir(directory);
    if (!dir)
    {
        report_error(directory);
        return (0);
    }
    while (1)
    {
        errno = 0;
        item = readdir(dir);
        if (!item)
        {
            if (errno != 0)
                report_error(directory);
            break ;
        }
        if (visible_name(item->d_name, flags))
        {
            path = join_path(directory, item->d_name);
            if (make_entry(path, item->d_name, &entry))
                list_push(list, &entry);
            free(path);
        }
    }
    if (closedir(dir) == -1)
        report_error(directory);
    return (1);
}

/* ---------------------------------------------------------- */
/* Permisos                                                   */
/* ---------------------------------------------------------- */

static char file_type(mode_t mode)
{
    if (S_ISDIR(mode))
        return ('d');
    if (S_ISLNK(mode))
        return ('l');
    if (S_ISCHR(mode))
        return ('c');
    if (S_ISBLK(mode))
        return ('b');
    if (S_ISFIFO(mode))
        return ('p');
    if (S_ISSOCK(mode))
        return ('s');
    return ('-');
}

static void permissions(mode_t mode, char str[11])
{
    str[0] = file_type(mode);
    str[1] = (mode & S_IRUSR) ? 'r' : '-';
    str[2] = (mode & S_IWUSR) ? 'w' : '-';
    str[3] = (mode & S_IXUSR) ? 'x' : '-';
    str[4] = (mode & S_IRGRP) ? 'r' : '-';
    str[5] = (mode & S_IWGRP) ? 'w' : '-';
    str[6] = (mode & S_IXGRP) ? 'x' : '-';
    str[7] = (mode & S_IROTH) ? 'r' : '-';
    str[8] = (mode & S_IWOTH) ? 'w' : '-';
    str[9] = (mode & S_IXOTH) ? 'x' : '-';
    if (mode & S_ISUID)
        str[3] = (mode & S_IXUSR) ? 's' : 'S';
    if (mode & S_ISGID)
        str[6] = (mode & S_IXGRP) ? 's' : 'S';
    if (mode & S_ISVTX)
        str[9] = (mode & S_IXOTH) ? 't' : 'T';
    str[10] = '\0';
}

/* ---------------------------------------------------------- */
/* Ancho visual de los nombres                                */
/* ---------------------------------------------------------- */

static int display_width(const char *text)
{
    mbstate_t state;
    wchar_t   wc;
    size_t    length;
    int       width;
    int       current;

    memset(&state, 0, sizeof(state));
    width = 0;
    while (*text)
    {
        length = mbrtowc(&wc, text, MB_CUR_MAX, &state);
        if (length == (size_t)-1 || length == (size_t)-2)
        {
            memset(&state, 0, sizeof(state));
            text++;
            width++;
        }
        else if (length == 0)
            break ;
        else
        {
            current = wcwidth(wc);
            width += (current >= 0) ? current : 1;
            text += length;
        }
    }
    return (width);
}

static void print_spaces(int count)
{
    while (count > 0)
    {
        putchar(' ');
        count--;
    }
}

/* ---------------------------------------------------------- */
/* Propietario, grupo y tamaños                               */
/* ---------------------------------------------------------- */

static char *owner_name(uid_t uid)
{
    struct passwd *pw;
    char          buffer[64];

    pw = getpwuid(uid);
    if (pw)
        return (ft_strdup(pw->pw_name));
    snprintf(buffer, sizeof(buffer), "%ju", (uintmax_t)uid);
    return (ft_strdup(buffer));
}

static char *group_name(gid_t gid)
{
    struct group *gr;
    char         buffer[64];

    gr = getgrgid(gid);
    if (gr)
        return (ft_strdup(gr->gr_name));
    snprintf(buffer, sizeof(buffer), "%ju", (uintmax_t)gid);
    return (ft_strdup(buffer));
}

static void size_string(
    const t_entry *entry,
    char *buffer,
    size_t capacity)
{
    if (S_ISCHR(entry->info.st_mode)
        || S_ISBLK(entry->info.st_mode))
    {
        snprintf(buffer, capacity, "%ju, %ju",
            (uintmax_t)major(entry->info.st_rdev),
            (uintmax_t)minor(entry->info.st_rdev));
    }
    else
    {
        snprintf(buffer, capacity, "%jd",
            (intmax_t)entry->info.st_size);
    }
}

static void calculate_widths(
    const t_list *list,
    t_widths *widths)
{
    size_t i;
    int    width;
    char   buffer[128];
    char   *name;

    widths->links = 1;
    widths->owner = 1;
    widths->group = 1;
    widths->size = 1;
    i = 0;
    while (i < list->count)
    {
        snprintf(buffer, sizeof(buffer), "%ju",
            (uintmax_t)list->items[i].info.st_nlink);
        width = (int)strlen(buffer);
        if (width > widths->links)
            widths->links = width;
        name = owner_name(list->items[i].info.st_uid);
        width = display_width(name);
        free(name);
        if (width > widths->owner)
            widths->owner = width;
        name = group_name(list->items[i].info.st_gid);
        width = display_width(name);
        free(name);
        if (width > widths->group)
            widths->group = width;
        size_string(&list->items[i], buffer, sizeof(buffer));
        width = (int)strlen(buffer);
        if (width > widths->size)
            widths->size = width;
        i++;
    }
}

/* ---------------------------------------------------------- */
/* Formato largo                                              */
/* ---------------------------------------------------------- */

static void format_date(
    time_t modification,
    char *buffer,
    size_t size)
{
    time_t    now;
    struct tm *tm_info;
    double    age;

    now = time(NULL);
    tm_info = localtime(&modification);
    if (!tm_info)
    {
        snprintf(buffer, size, "??? ?? ??:??");
        return ;
    }
    age = difftime(now, modification);
    if (age < 0 || age >= 15778476.0)
        strftime(buffer, size, "%b %e  %Y", tm_info);
    else
        strftime(buffer, size, "%b %e %H:%M", tm_info);
}

static void print_link_target(const t_entry *entry)
{
    char    *buffer;
    size_t  capacity;
    ssize_t length;

    if (!S_ISLNK(entry->info.st_mode))
        return ;
    capacity = 256;
    while (1)
    {
        buffer = xmalloc(capacity + 1);
        length = readlink(entry->path, buffer, capacity);
        if (length == -1)
        {
            free(buffer);
            report_error(entry->path);
            return ;
        }
        if ((size_t)length < capacity)
        {
            buffer[length] = '\0';
            printf(" -> %s", buffer);
            free(buffer);
            return ;
        }
        free(buffer);
        if (capacity > (SIZE_MAX - 1) / 2)
            memory_error();
        capacity *= 2;
    }
}

static void print_long(
    const t_entry *entry,
    const t_widths *widths)
{
    char mode[11];
    char date[128];
    char size[128];
    char *owner;
    char *group;

    permissions(entry->info.st_mode, mode);
    format_date(entry->info.st_mtime, date, sizeof(date));
    size_string(entry, size, sizeof(size));
    owner = owner_name(entry->info.st_uid);
    group = group_name(entry->info.st_gid);
    printf("%s %*ju ", mode, widths->links,
        (uintmax_t)entry->info.st_nlink);
    printf("%s", owner);
    print_spaces(widths->owner - display_width(owner) + 1);
    printf("%s", group);
    print_spaces(widths->group - display_width(group) + 1);
    printf("%*s %s %s",
        widths->size, size, date, entry->name);
    print_link_target(entry);
    putchar('\n');
    free(owner);
    free(group);
}

/* ---------------------------------------------------------- */
/* Salida en columnas                                         */
/* ---------------------------------------------------------- */

static int terminal_width(void)
{
    struct winsize window;
    const char     *env;
    char           *end;
    long           value;

    env = getenv("COLUMNS");
    if (env && *env)
    {
        errno = 0;
        value = strtol(env, &end, 10);
        if (!errno && *end == '\0'
            && value > 0 && value <= INT_MAX)
            return ((int)value);
    }
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &window) == 0
        && window.ws_col > 0)
        return ((int)window.ws_col);
    return (80);
}

static int column_width(
    const t_list *list,
    size_t rows,
    size_t column)
{
    size_t row;
    size_t index;
    int    width;
    int    current;

    row = 0;
    width = 0;
    while (row < rows)
    {
        index = row + column * rows;
        if (index >= list->count)
            break ;
        current = display_width(list->items[index].name);
        if (current > width)
            width = current;
        row++;
    }
    return (width);
}

static int columns_fit(
    const t_list *list,
    size_t columns,
    int screen_width)
{
    size_t rows;
    size_t column;
    size_t used_columns;
    size_t total;

    rows = (list->count + columns - 1) / columns;
    used_columns = (list->count + rows - 1) / rows;
    total = 0;
    column = 0;
    while (column < used_columns)
    {
        total += (size_t)column_width(list, rows, column);
        if (column + 1 < used_columns)
            total += 2;
        if (total > (size_t)screen_width)
            return (0);
        column++;
    }
    return (1);
}

static void print_columns(const t_list *list)
{
    size_t columns;
    size_t rows;
    size_t row;
    size_t column;
    size_t index;
    int    screen_width;
    int    padding;

    if (list->count == 0)
        return ;
    screen_width = terminal_width();
    columns = list->count;

    /*
    ** Cada columna necesita al menos un carácter
    ** y dos espacios de separación.
    */
    if (columns > ((size_t)screen_width + 2) / 3)
        columns = ((size_t)screen_width + 2) / 3;
    if (columns == 0)
        columns = 1;
    while (columns > 1
        && !columns_fit(list, columns, screen_width))
        columns--;

    rows = (list->count + columns - 1) / columns;
    row = 0;
    while (row < rows)
    {
        column = 0;
        while (column < columns)
        {
            index = row + column * rows;
            if (index >= list->count)
                break ;
            printf("%s", list->items[index].name);
            if (column + 1 < columns
                && index + rows < list->count)
            {
                padding = column_width(list, rows, column)
                    - display_width(list->items[index].name) + 2;
                print_spaces(padding);
            }
            column++;
        }
        putchar('\n');
        row++;
    }
}

/* ---------------------------------------------------------- */
/* Selección del formato de salida                            */
/* ---------------------------------------------------------- */

static void print_entries(
    const t_list *list,
    const t_flags *flags,
    int directory_contents)
{
    size_t    i;
    intmax_t  blocks;
    t_widths  widths;

    if (!flags->l)
    {
        if (isatty(STDOUT_FILENO))
            print_columns(list);
        else
        {
            i = 0;
            while (i < list->count)
                printf("%s\n", list->items[i++].name);
        }
        return ;
    }

    calculate_widths(list, &widths);
    if (directory_contents)
    {
        blocks = 0;
        i = 0;
        while (i < list->count)
        {
            blocks += (intmax_t)list->items[i].info.st_blocks;
            i++;
        }
        printf("total %jd\n", (blocks + 1) / 2);
    }
    i = 0;
    while (i < list->count)
    {
        print_long(&list->items[i], &widths);
        i++;
    }
}

/* ---------------------------------------------------------- */
/* Recursividad                                               */
/* ---------------------------------------------------------- */

static int dot_directory(const char *name)
{
    return (strcmp(name, ".") == 0
        || strcmp(name, "..") == 0);
}

static void list_directory(
    const char *directory,
    const t_flags *flags,
    int header,
    int *first)
{
    t_list list;
    size_t i;

    list_init(&list);
    if (header)
    {
        if (!*first)
            putchar('\n');
        printf("%s:\n", directory);
        *first = 0;
    }
    if (!read_directory(directory, flags, &list))
    {
        list_free(&list);
        return ;
    }
    sort_list(&list, flags);
    print_entries(&list, flags, 1);
    *first = 0;
    if (flags->R)
    {
        i = 0;
        while (i < list.count)
        {
            if (S_ISDIR(list.items[i].info.st_mode)
                && !dot_directory(list.items[i].name))
            {
                list_directory(
                    list.items[i].path,
                    flags,
                    1,
                    first);
            }
            i++;
        }
    }
    list_free(&list);
}

/* ---------------------------------------------------------- */
/* Archivos y directorios recibidos como argumentos            */
/* ---------------------------------------------------------- */

static void process_paths(
    char **paths,
    int count,
    const t_flags *flags)
{
    t_list      files;
    t_list      dirs;
    t_entry     entry;
    struct stat target;
    int         i;
    int         first;
    int         header;

    list_init(&files);
    list_init(&dirs);
    i = 0;
    while (i < count)
    {
        /*
        ** Conservamos el argumento completo para imprimir:
        **
        ** ./ft_ls src/main.c
        ** -> src/main.c
        */
        if (make_entry(paths[i], paths[i], &entry))
        {
            /*
            ** Sin -l, un enlace explícito a un directorio
            ** se trata como directorio.
            **
            ** Los enlaces encontrados durante -R no se siguen.
            */
            if (!flags->l && S_ISLNK(entry.info.st_mode)
                && stat(paths[i], &target) == 0
                && S_ISDIR(target.st_mode))
            {
                entry.info = target;
            }
            if (S_ISDIR(entry.info.st_mode))
                list_push(&dirs, &entry);
            else
                list_push(&files, &entry);
        }
        i++;
    }
    sort_list(&files, flags);
    sort_list(&dirs, flags);
    first = 1;
    if (files.count > 0)
    {
        print_entries(&files, flags, 0);
        first = 0;
    }
    header = (count > 1 || flags->R);
    i = 0;
    while ((size_t)i < dirs.count)
    {
        list_directory(
            dirs.items[i].path,
            flags,
            header,
            &first);
        i++;
    }
    list_free(&files);
    list_free(&dirs);
}

/* ---------------------------------------------------------- */
/* Main                                                       */
/* ---------------------------------------------------------- */

int main(int argc, char **argv)
{
    t_flags flags;
    char    **paths;
    int     path_count;
    int     first;

    setlocale(LC_ALL, "");
    memset(&flags, 0, sizeof(flags));
    paths = xmalloc(sizeof(*paths) * (size_t)argc);
    if (!parse_args(
            argc,
            argv,
            &flags,
            paths,
            &path_count))
    {
        free(paths);
        return (2);
    }
    if (path_count == 0)
    {
        first = 1;
        list_directory(".", &flags, flags.R, &first);
    }
    else
        process_paths(paths, path_count, &flags);
    free(paths);
    if (fflush(stdout) == EOF)
        g_status = 1;
    return (g_status);
}