# ft_ls

A reimplementation of the Unix `ls` command written in C.

`ft_ls` reproduces the essential behavior of `ls` while implementing the mandatory options required by the project:

```text
-l  -R  -a  -r  -t
```

The project focuses on filesystem traversal, file metadata, sorting, recursive directory processing, formatted output, error handling, and memory management.

---

## Features

### Supported options

| Option | Description |
|:------:|-------------|
| `-l` | Display entries using long format |
| `-R` | Recursively list subdirectories |
| `-a` | Include hidden entries |
| `-r` | Reverse the sorting order |
| `-t` | Sort entries by modification time |

Options can be combined:

```bash
./ft_ls -laRtr
```

---

## Build

Compile the project with:

```bash
make
```

The project is compiled using:

```text
-Wall -Wextra -Werror
```

Available Makefile rules:

```bash
make
make clean
make fclean
make re
```

### `clean`

Removes object files while keeping the executable:

```bash
make clean
```

### `fclean`

Removes object files and the executable:

```bash
make fclean
```

### `re`

Performs a complete rebuild:

```bash
make re
```

---

## Usage

Basic listing:

```bash
./ft_ls
```

List a specific directory:

```bash
./ft_ls src
```

Long format:

```bash
./ft_ls -l
```

Include hidden entries:

```bash
./ft_ls -a
```

Recursive listing:

```bash
./ft_ls -R
```

Sort by modification time:

```bash
./ft_ls -t
```

Reverse sorting:

```bash
./ft_ls -r
```

Combine multiple options:

```bash
./ft_ls -laRtr
```

Multiple files and directories can also be provided:

```bash
./ft_ls -laRtr Makefile include src
```

---

## How it works

The high-level execution flow is:

```text
main()
│
├── parse_flags()
│
└── process_paths()
    │
    ├── classify_paths()
    │
    ├── print_errors()
    │
    ├── process_files()
    │
    └── process_directories()
        │
        └── read_directory()
            │
            ├── read_entries()
            ├── sort_list()
            ├── print_total()
            ├── print_names()
            └── recurse_directories()
```

### 1. Option parsing

`parse_flags()` parses the supported options using `getopt()`.

An invalid option is treated as a serious error and stops execution before paths are processed.

### 2. Path classification

`classify_paths()` uses `lstat()` to classify command-line arguments into:

- files;
- directories;
- inaccessible paths.

Files and directories are stored separately so they can be sorted and displayed correctly.

### 3. Directory processing

Directories are opened with `opendir()` and traversed with `readdir()`.

Each discovered entry stores:

- its name;
- its complete path;
- its `struct stat` metadata;
- a pointer to the next entry.

With `-R`, eligible subdirectories are processed recursively.

The special entries:

```text
.
..
```

are never recursively traversed.

Hidden directories are recursively processed only when `-a` is enabled.

---

## Sorting

`ft_ls` supports four sorting modes.

| `-t` | `-r` | Result |
|:----:|:----:|--------|
| No | No | Alphabetical ascending |
| No | Yes | Alphabetical descending |
| Yes | No | Modification time, newest first |
| Yes | Yes | Modification time, oldest first |

Time sorting uses both:

```c
st_mtim.tv_sec
st_mtim.tv_nsec
```

If two entries have the same modification timestamp, their names are used as a tie-breaker.

---

## Long format

The `-l` option displays detailed filesystem information:

```text
type permissions links owner group size date name
```

Example:

```text
-rw-r--r-- 1 user group 1371 Nov  9 16:41 Makefile
```

The output includes:

- file type;
- Unix permission bits;
- hard-link count;
- owner;
- group;
- file size;
- modification date;
- file name;
- symbolic-link target when applicable.

Column widths are calculated before printing so values remain properly aligned.

---

## File types

`ft_ls` identifies the main Unix filesystem entry types.

| Character | Type |
|:---------:|------|
| `-` | Regular file |
| `d` | Directory |
| `l` | Symbolic link |
| `c` | Character device |
| `b` | Block device |
| `p` | FIFO / named pipe |
| `s` | Unix socket |
| `?` | Unknown or unsupported type |

---

## Permissions

Standard permission characters are supported:

```text
r  read
w  write
x  execute
-  permission not enabled
```

Special permission bits are also represented correctly:

| Character | Meaning |
|:---------:|---------|
| `s` | setuid/setgid with execute permission |
| `S` | setuid/setgid without execute permission |
| `t` | sticky bit with execute permission |
| `T` | sticky bit without execute permission |

For example:

```text
-rwsr-xr-x
-rwxr-sr-x
drwxrwxrwt
drwxrw-rwT
```

---

## Symbolic links

Symbolic links are inspected using `lstat()` and their targets are obtained with `readlink()`.

Example:

```text
lrwxrwxrwx 1 user group 4 Nov 11 09:11 link -> file
```

Broken symbolic links are also handled:

```text
link -> DOES_NOT_EXIST
```

---

## Device files

Character and block devices use their device identifier rather than a normal file size.

`ft_ls` extracts the major and minor device numbers from:

```c
st_rdev
```

using:

```c
major()
minor()
```

For example:

```text
crw-rw-rw- 1 root root 1, 3 /dev/null
```

---

## Dates

Modification dates follow the traditional `ls -l` presentation.

Recent files display the time:

```text
Nov 11 10:30
```

Old files display the year:

```text
Jan 10  2020
```

Files sufficiently far in the future also display the year.

---

## Total blocks

For directory listings using `-l`, `ft_ls` calculates:

```text
total N
```

from the `st_blocks` value of visible entries.

`st_blocks` is expressed in 512-byte units, so it is converted to the block representation used by the expected `ls` output.

Hidden entries contribute to the total only when `-a` is enabled.

---

## Error handling

`ft_ls` distinguishes between successful execution, minor errors, and serious errors.

| Exit status | Meaning |
|:-----------:|---------|
| `0` | Successful execution |
| `1` | Minor error discovered during processing |
| `2` | Serious error involving an explicit path or invalid option |

### Success

```bash
./ft_ls Makefile
echo $?
```

```text
0
```

### Serious error

An explicitly requested path that cannot be accessed:

```bash
./ft_ls DOES_NOT_EXIST
echo $?
```

returns:

```text
2
```

Valid arguments continue to be processed even if another explicit argument fails:

```bash
./ft_ls DOES_NOT_EXIST Makefile
```

### Minor recursive error

If a directory discovered while processing `-R` cannot be opened, processing continues with the remaining directories and the final status is `1` unless a more serious error has also occurred.

When several errors occur, the highest error level is preserved.

---

## Memory management

Dynamic memory is used for:

- entry nodes;
- entry names;
- complete paths;
- error nodes;
- error paths.

Dedicated cleanup functions release all allocated resources:

```c
free_entries()
free_errors()
```

Allocation failure paths also release any memory allocated earlier in the same operation.

The project has been tested with Valgrind using normal execution, recursive traversal, invalid paths, symbolic links, permission errors, and combined mandatory options.

Example:

```bash
valgrind \
    --leak-check=full \
    --show-leak-kinds=all \
    --track-origins=yes \
    ./ft_ls -laRtr .
```

Expected result:

```text
in use at exit: 0 bytes in 0 blocks
All heap blocks were freed -- no leaks are possible
ERROR SUMMARY: 0 errors
```

---

## Project structure

```text
ft_ls/
├── include/
│   └── ft_ls.h
├── src/
│   ├── add_entry.c
│   ├── add_error.c
│   ├── add_path.c
│   ├── append_entry.c
│   ├── classify_paths.c
│   ├── free_entries.c
│   ├── free_errors.c
│   ├── get_widths.c
│   ├── join_path.c
│   ├── main.c
│   ├── parse_flags.c
│   ├── print_date.c
│   ├── print_entry.c
│   ├── print_errors.c
│   ├── print_file_type.c
│   ├── print_group.c
│   ├── print_names.c
│   ├── print_owner.c
│   ├── print_permissions.c
│   ├── print_size.c
│   ├── print_symlink.c
│   ├── print_total.c
│   ├── process_directories.c
│   ├── process_files.c
│   ├── process_paths.c
│   ├── read_directory.c
│   ├── read_file.c
│   ├── sort_entries.c
│   └── sort_list.c
├── Makefile
└── README.md
```

---

## Main data structures

### `t_ls_entry`

Represents one filesystem entry.

Contains:

```text
name
path
metadata
next
```

### `t_ls_list`

Linked-list container containing:

```text
first
last
```

### `t_ls_flags`

Stores the state of the supported options:

```text
l
R
a
r
t
```

### `t_ls_error`

Stores information about an inaccessible path:

```text
path
error_num
next
```

### `t_ls_error_list`

Linked-list container for errors:

```text
first
last
```

### `t_ls_widths`

Stores the calculated widths used by long-format output:

```text
links
owner
group
size
```

---

## Implementation highlights

This project provides practical experience with several Unix and C concepts:

- filesystem traversal with `opendir()` and `readdir()`;
- metadata retrieval using `lstat()`;
- symbolic links with `readlink()`;
- Unix file types and permission bits;
- `uid_t` / `gid_t` resolution with `getpwuid()` and `getgrgid()`;
- character and block device numbers;
- modification timestamps with nanosecond precision;
- linked lists;
- dynamic memory management;
- sorting algorithms;
- recursive filesystem traversal;
- command-line parsing with `getopt()`;
- Unix process exit statuses.

---

## Testing

The implementation has been compared against the system `ls` using the `C` locale:

```bash
LC_ALL=C ls ...
LC_ALL=C ./ft_ls ...
```

The test suite covered, among other cases:

- all mandatory options individually;
- combined options such as `-laRtr`;
- multiple files and directories;
- hidden files and directories;
- recursive traversal;
- empty directories;
- zero-byte files;
- modification-time ties;
- old and future timestamps;
- valid and broken symbolic links;
- FIFOs;
- Unix sockets;
- character devices;
- block devices;
- setuid;
- setgid;
- sticky bits;
- inaccessible explicit directories;
- inaccessible recursively discovered directories;
- invalid paths;
- invalid options;
- exit statuses `0`, `1`, and `2`;
- memory cleanup on successful and failing execution paths.

---

## Mandatory scope

The project implements:

```text
-l
-R
-a
-r
-t
```

The focus of the implementation is correctness of the mandatory behavior rather than optional extensions.

---

## Author

Developed as part of the **42 Outer Core `ft_ls` project**.