#include "shell.h"

/**
* main - Main function of the shell
*
* Description: Infinite loop that displays a prompt, reads user input,
* parses it, and executes the command. Continues until EOF (Ctrl+D).
*
* Return: 0 on success
*/
int main(void)
{
char *line;
char **args;
int status;

status = 1;

while (status)
{
printf("simple_shell$ ");
fflush(stdout);
line = read_line();
args = split_line(line);
status = execute(args);
free(line);
free(args);
}

return (0);

