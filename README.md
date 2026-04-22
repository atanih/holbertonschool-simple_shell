# Simple Shell

I'm working on this for Holberton—it's a small shell in C that reads what you type and runs programs, kind of like a stripped-down `sh`. Right now the README is mostly so the repo meets Task 0; we'll keep updating it as we add more behavior.

## What it does

The program is called *`hsh`**. It sits in a loop: show a prompt, read a line, figure out the command, then either handle built-ins in the parent or `fork` / `execve` for normal programs. 

We're supposed to build on Ubuntu 20.04 with gcc and these flags:

```bash

gcc -Wall -Werror -Wextra -pedantic -std=gnu89 *.c -o hsh