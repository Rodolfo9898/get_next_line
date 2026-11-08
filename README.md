*This project has been created as part of the 42 curriculum by rperez-t.*

# get_next_line

## Description

`get_next_line` is a C function that reads a file descriptor one line at a time:

```c
char	*get_next_line(int fd);
```

Each call returns the next line, including its terminating `\n`, except for a last line that does not end with one. The line is allocated with `malloc`, and the caller must `free` it. The function returns `NULL` when there is nothing left to read or when an error occurs (invalid fd, failed `read`, failed `malloc`). It works on regular files and on standard input.

The goal of the project is to learn about static variables, buffered reading with `read()`, and careful memory management. Only `read`, `malloc` and `free` are allowed.

The bonus version (`make bonus`) can read from several file descriptors at the same time, for example fd 3, then 4, then 3 again, without mixing up their lines. It still uses a single static variable.

## Instructions

With the Makefile:

```sh
make                    # builds get_next_line.a (mandatory part)
make bonus              # builds get_next_line_bonus.a (several fds at once)
make BUFFER_SIZE=42     # rebuild with another buffer size
make clean / fclean     # remove objects / objects and archives

cc -Wall -Wextra -Werror -I include main.c get_next_line.a
```

The output is coloured (`mk/colors.mk`): commands in green, errors in red, warnings in
yellow. `clean` and `fclean` say in blue what they remove, or warn in yellow when there
is nothing to remove.

Or compile the sources directly together with your own `main`:

```sh
# mandatory part
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 -I include sources/*.c main.c

# bonus part (several fds at once): the same files, built with BONUS_MODE=1
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 -D BONUS_MODE=1 -I include sources/*.c main.c
```

`BUFFER_SIZE` is the number of bytes asked from `read()` each time. It is optional and defaults to 42. A value of 0 or less makes the function return `NULL`.

The mandatory version keeps the bytes of only one fd at a time: calling it on another fd drops what was stored for the previous one, so read each fd to the end before switching. Use the bonus version to read several fds in turn.

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

`read()` returns blocks of `BUFFER_SIZE` bytes that do not line up with lines. One block can hold several lines, or only part of one. What was read past the end of the returned line must therefore survive until the next call. It is kept in a **static "stash"**: a single `t_stash` in the mandatory part, and in the bonus a linked list of `t_stash` nodes, one per fd that has bytes stored, reached from one static pointer. The list has no fixed size, so any fd the system hands out works (a fixed array would have to guess the fd limit, which Linux sets at run time). A node is added on the first call for its fd and removed as soon as its buffer is freed, so finished fds cost nothing. A stash is a growable buffer: `data` is the allocation, `cap` its size, and the bytes not returned yet are the `len` bytes starting at `data + start`.

The code is laid out like this: one header, and the same sources for both libraries.

```
include/get_next_line.h                  every prototype, grouped by file
include/core/libs.h        system headers
include/core/defines.h     BONUS_MODE (0 by default) and BUFFER_SIZE
include/core/structs.h     t_stash
sources/get_next_line.c                  get_next_line() alone
sources/get_next_line_utils.c            gnl_stash_clear, gnl_extract_line, gnl_read_until_newline
                                         (+ static gnl_stash_move, gnl_stash_reserve)
sources/get_next_line_bonus.c            only with BONUS_MODE: gnl_bonus_get_next_line
                                         (+ static gnl_get_stash, gnl_remove_stash)
```

`make bonus` compiles the same files with `-D BONUS_MODE=1` (objects in `objects_bonus/`). After checking `fd` and `BUFFER_SIZE`, `get_next_line()` then hands over to `gnl_bonus_get_next_line()` under `if (BONUS_MODE)`. That function is declared in both builds and defined only in the bonus one; in the mandatory build the branch is dead, and the compiler drops the call. Both builds have a single static variable, the `t_stash` of `get_next_line()`: in the bonus, its `next` field is the head of the list of stashes, one per fd. `gnl_get_stash` finds or adds the node of an fd, `gnl_remove_stash` unlinks and frees it.

Each call to `get_next_line(fd)`:

1. **Validates** the input. A negative fd or a bad `BUFFER_SIZE` returns `NULL`. `read(fd, 0, 0)` then checks that the fd is readable. If not (for example, it was closed), the stash of that fd is freed, so a file reopened later on the same fd starts from a clean state. The mandatory stash records which fd its bytes came from: a bad call on another fd keeps them, and a call on another, valid fd drops them before reading, so lines from two fds are never mixed (reading several fds at once is what the bonus adds).
2. **Fills the stash** (`gnl_read_until_newline`). While the stash contains no `\n` and `read()` has not hit end of file, it makes room for one block (`gnl_stash_reserve`) and reads it directly after the stored bytes. Only the new bytes are searched for `\n`, because the older ones are already known to have none. It stops as soon as a newline arrives, so it reads as little as possible rather than the whole file.
   If the stash is still empty at end of file, there is nothing left: `get_next_line()` frees it and returns `NULL`.
3. **Extracts the line** (`gnl_extract_line`). It copies the stored bytes up to and including the first `\n`, or all of them at end of file, into a new string, then simply moves `start` past them. When the last stored byte is returned, the buffer is freed (and, in the bonus, the node of that fd).

**Why this design:**
- The static stash is the simplest structure that meets "read as little as possible" and keeps state between calls without a global variable.
- Returning a line only moves `start`, so the rest is never copied once per line.
- When there is no room after the stored bytes, `gnl_stash_reserve` moves them to the front of the same block if they fit there, and only otherwise to a new block twice as large as needed. Each byte is therefore copied only a constant number of times on average, and reading is linear in the file size even for a very long line with `BUFFER_SIZE=1`. A plain string joined with every block would copy the whole stash at each `read()`, which is quadratic.
- The buffer size is tied to the bytes still stored, not to the file size, so a long file does not make it grow. After a very long line the block can be much larger than what is left; once it is over four times larger than needed, the next refill moves the bytes to a block of the right size, so that memory is given back.

**Memory ownership:** the stash owns its buffer. A failed `read()` frees it and returns `NULL`. A failed `malloc` also returns `NULL` but keeps the stash: the fd has already moved past the stored bytes, so freeing them would lose data, and a later call returns the same line again. A caller that wants to tell a `malloc` failure from end of file can check `errno`, which `malloc` sets to `ENOMEM`. When the last stored byte is returned, the buffer is freed.

## Resources

- `man 2 read`, `man 3 malloc`, `man 3 free`
- [Static variables in C](https://en.cppreference.com/w/c/language/storage_duration)
- [File descriptors](https://en.wikipedia.org/wiki/File_descriptor)
- Valgrind manual: <https://valgrind.org/docs/manual/quick-start.html>

**Use of AI.** An AI assistant (Claude) was used for review,it checked the finished library against the subject and the Norm, pointed out failures and explained why they did not match the subject.
