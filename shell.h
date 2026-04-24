#ifndef SHELL_H
#define SHELL_H

#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int hsh_loop(const char *argv0, char **envp);
char **line_to_argv(const char *line, int *ac, char **out_linebuf);
void free_line_args(char **argv, int ac, char *linebuf);
char *path_resolve(const char *cmd, char **envp);
int launch_child(const char *fullpath, char **argv, char **envp);
int handle_builtin(int ac, char **av, char **envp, int *should_exit, int *exit_st);

#endif /* SHELL_H */
