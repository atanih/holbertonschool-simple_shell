#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

char *read_line(void);
char **split_line(char *line);
int launch(char **args);
int execute(char **args);

#endif

