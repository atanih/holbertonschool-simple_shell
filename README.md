# Simple Shell

Tiny shell for Holberton's simple_shell project. You get a prompt, type a command, it either runs a built-in in the parent or forks and execve's the real binary. Nothing fancy: no pipes, no rediretion, no job control.

## Build

Ubuntu 20.04, gcc with the ususal flags:

```bash
gcc -Wall -Werror -Wextra -pedantic -std=gnu89 *.c -o hsh
```

That drops an executable named `hsh` next to the sources. I don't commit the binary; rebuld after pulls.

## Run

Interactive:

```bash
./hsh
```

You'll see `($) ` when stdin is a tty. Ctrl+D ends input (we print a newline in interactive mode before quiting).

One-shot from stdin:

```bash
echo "/bin/ls" | ./hsh
```

No prompt in that mode, same as you'd excpect.

## Behavior (what actually works)

- **Built-ins:** `exit` (no args required for our version) and `env` (dumps the enviroment, one variable per line).
- **Everything else:** resolved with **PATH** if it's not already a path with a slash; then fork + wait + **execve** with the same environment **main** got.
- **Errors:** missing commands look like `./hsh: 1: cmd: not found` (first field is whatever you invoked us as, second is the line number).

We don't handle quotes, globbing, semicolons, or `cd` unless we add it later.

## Where things live

- `main.c` — entry, passes argv[0] and the environment into the loop  
- `hsh.c` — prompt, getline, built-in vs external dispatch  
- `parse.c` — line → argv  
- `path.c` — PATH walk, helpers  
- `child.c` — fork, execve, wait  
- `builtin.c` — `exit` and `env` without execing  
- `shell.h` — headers / prototypes  

More detial in `man_1_simple_shell`. Who worked on it is in [`AUTHORS`](AUTHORS).
