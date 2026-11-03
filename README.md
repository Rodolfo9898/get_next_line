*This project has been created as part of the 42 curriculum by rperez-t.*

# get_next_line

## Description

`get_next_line` is a C function that reads a file descriptor one line at a time:

```c
char	*get_next_line(int fd);
```

Each call returns the next line, including its terminating `\n`, except for a last line that does not end with one. The line is allocated with `malloc`, and the caller must `free` it. The function returns `NULL` when there is nothing left to read or when an error occurs (invalid fd, failed `read`, failed `malloc`). It works on regular files and on standard input.

The goal of the project is to learn about static variables, buffered reading with `read()`, and careful memory management. Only `read`, `malloc` and `free` are allowed.

The bonus version (`*_bonus.c`/`*_bonus.h`) can read from several file descriptors at the same time, for example fd 3, then 4, then 3 again, without mixing up their lines. It still uses a single static variable.

## Instructions

With the Makefile:

```sh
make                    # builds get_next_line.a (mandatory part)
make bonus              # builds get_next_line_bonus.a (several fds at once)
make re BUFFER_SIZE=42  # rebuild with another buffer size
make clean / fclean     # remove objects / objects and archives

cc -Wall -Wextra -Werror main.c get_next_line.a
```

Or compile the sources directly together with your own `main`:

```sh
# mandatory part
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 get_next_line.c get_next_line_utils.c main.c

# bonus part (several fds at once)
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 get_next_line_bonus.c get_next_line_utils_bonus.c main.c
```

`BUFFER_SIZE` is the number of bytes asked from `read()` each time. It is optional and defaults to 1. A value of 0 or less makes the function return `NULL`.

Minimal `main.c`:

```c
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include "get_next_line.h"

int	main(int argc, char **argv)
{
	int		fd;
	char	*line;

	fd = 0;
	if (argc > 1)
		fd = open(argv[1], O_RDONLY);
	line = get_next_line(fd);
	while (line)
	{
		printf("%s", line);
		free(line);
		line = get_next_line(fd);
	}
	return (0);
}
```

## Algorithm

`read()` returns blocks of `BUFFER_SIZE` bytes that do not line up with lines. One block can hold several lines, or only part of one. What was read past the end of the returned line must therefore survive until the next call. It is kept in a **static "stash"**: a `char *` in the mandatory part, and an array `char *stash[FDS_TO_OPEN]` (one stash per fd) in the bonus.

Each call to `get_next_line(fd)`:

1. **Validates** the input. A negative fd, an fd outside the bonus array, or a bad `BUFFER_SIZE` returns `NULL`. `read(fd, 0, 0)` then checks that the fd is readable. If not (for example, it was closed), the stash is freed, so a file reopened later on the same fd starts from a clean state.
2. **Fills the stash** (`ft_read_file`). While the stash contains no `\n` and `read()` has not hit end of file, it reads one block and appends it to the stash. Only the new block is searched for `\n`, because the rest of the stash is already known to have none. It stops as soon as a newline arrives, so it reads as little as possible rather than the whole file.
3. **Extracts the line** (`ft_isolate_single_line`). It copies the stash up to and including the first `\n`, or the whole stash at end of file. An empty stash means there is nothing left, so it frees the stash and returns `NULL`.
4. **Keeps the rest** (`ft_get_rest_of_lines`). What follows the `\n` becomes the new stash, and the old one is freed. When nothing follows, the stash becomes `NULL`.

**Memory ownership** is kept simple on purpose. Every helper that receives the stash either returns a new stash or frees the old one, and it does so even when `malloc` or `read` fails. A failure anywhere therefore returns `NULL` without leaking or reusing freed memory.

**Why this design:**
- The static stash is the simplest structure that meets "read as little as possible" and keeps state between calls without a global variable.
- A plain string, rather than a linked list of blocks, keeps the line search and extraction easy to follow.

Its known cost is that each `read()` copies the whole stash once. A very long line read with a tiny `BUFFER_SIZE` therefore takes time quadratic in the line length. With a normal `BUFFER_SIZE` this does not matter.

## Resources

- `man 2 read`, `man 3 malloc`, `man 3 free`
- [Static variables in C](https://en.cppreference.com/w/c/language/storage_duration)
- [File descriptors](https://en.wikipedia.org/wiki/File_descriptor)
- Valgrind manual: <https://valgrind.org/docs/manual/quick-start.html>

### Use of AI

Claude Code (Anthropic) was used on 2026-11-03 to:
- review the project and test it: different `BUFFER_SIZE` values, edge-case files, valgrind, and simulated `malloc`/`read` failures;
- fix the problems the review found: a crash and leaks when `malloc` or `read` fails, an out-of-bounds access in the bonus for fds ≥ 1024, unclear variable names, dead code and unused headers;
- draft this README.

The algorithm and the original implementation are my own work. I checked and understood every change before submitting.
