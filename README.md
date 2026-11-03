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

`BUFFER_SIZE` is the number of bytes asked from `read()` each time. It is optional and defaults to 42. A value of 0 or less makes the function return `NULL`.

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

`read()` returns blocks of `BUFFER_SIZE` bytes that do not line up with lines. One block can hold several lines, or only part of one. What was read past the end of the returned line must therefore survive until the next call. It is kept in a **static "stash"**: a `t_stash` in the mandatory part, and an array `t_stash stash[FD_LIMIT]` (one stash per fd) in the bonus. A stash is a growable buffer: `data` is the allocation, `cap` its size, and the bytes not returned yet are the `len` bytes starting at `data + start`.

`get_next_line()` is the only function in `get_next_line.c` (and `get_next_line_bonus.c`). The four helpers live in `get_next_line_utils.c` (and `get_next_line_utils_bonus.c`) and are declared in the header.

Each call to `get_next_line(fd)`:

1. **Validates** the input. A negative fd, an fd outside the bonus array, or a bad `BUFFER_SIZE` returns `NULL`. `read(fd, 0, 0)` then checks that the fd is readable. If not (for example, it was closed), the stash is freed, so a file reopened later on the same fd starts from a clean state.
2. **Fills the stash** (`ft_read_until_newline`). While the stash contains no `\n` and `read()` has not hit end of file, it makes room for one block (`ft_stash_reserve`) and reads it directly after the stored bytes. Only the new bytes are searched for `\n`, because the older ones are already known to have none. It stops as soon as a newline arrives, so it reads as little as possible rather than the whole file.
3. **Extracts the line** (`ft_extract_line`). It copies the stored bytes up to and including the first `\n`, or all of them at end of file, into a new string, then simply moves `start` past them. An empty stash at end of file means there is nothing left, so it is freed and `NULL` is returned.

**Why this design:**
- The static stash is the simplest structure that meets "read as little as possible" and keeps state between calls without a global variable.
- Returning a line only moves `start`, so the rest is never copied once per line.
- When the buffer is full, `ft_stash_reserve` moves the stored bytes to a block twice as large as needed. Each byte is therefore copied only a constant number of times on average, and reading is linear in the file size even for a very long line with `BUFFER_SIZE=1`. A plain string joined with every block would copy the whole stash at each `read()`, which is quadratic.
- The buffer size is tied to the bytes still stored, not to the file size, so a long file does not make it grow.

**Memory ownership:** the stash owns its buffer. A failed `read()` frees it and returns `NULL`. A failed `malloc` returns `NULL` but keeps the stash intact, so no data is lost and a later call can continue. When the last stored byte is returned, the buffer is freed.

## Resources

- `man 2 read`, `man 3 malloc`, `man 3 free`
- [Static variables in C](https://en.cppreference.com/w/c/language/storage_duration)
- [File descriptors](https://en.wikipedia.org/wiki/File_descriptor)
- Valgrind manual: <https://valgrind.org/docs/manual/quick-start.html>

**Use of AI.** An AI assistant (Claude) reviewed the finished library against the subject, pointed out failures and explained why they did not match the subject, and proposed fixes.
