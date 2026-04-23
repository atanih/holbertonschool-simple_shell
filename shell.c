#include "shell.h"

/**
* read_line - Reads a line from standard input
*
* Description: Uses getline to read a full line of input from stdin.
* Handles EOF gracefully. Removes the trailing newline character.
*
* Return: Pointer to the allocated line (must be freed by caller)
*/
char *read_line(void)
{
char *line = NULL;
size_t len = 0;
ssize_t read_len;

read_len = getline(&line, &len, stdin);

if (read_len == -1)
{
free(line);
exit(EXIT_SUCCESS);
}

if (read_len > 0 && line[read_len - 1] == '\n')
line[read_len - 1] = '\0';

return (line);
}

/**
* split_line - Splits a line into tokens
* @line: The input line to tokenize
*
* Description: Uses strtok to split the input line by spaces and tabs.
* Allocates memory for an array of pointers to store each token.
* Returns an array terminated with NULL.
*
* Return: Pointer to array of token strings (must be freed by caller)
*/
char **split_line(char *line)
{
int bufsize = BUFSIZE, pos = 0;
char **tokens = malloc(bufsize * sizeof(char *));
char *token;

if (!tokens)
{
perror("malloc");
exit(EXIT_FAILURE);
}

token = strtok(line, DELIM);
while (token != NULL)
{
tokens[pos] = token;
pos++;

if (pos >= bufsize)
{
bufsize += BUFSIZE;
tokens = realloc(tokens, bufsize * sizeof(char *));
if (!tokens)
{
perror("realloc");
exit(EXIT_FAILURE);
}
}

token = strtok(NULL, DELIM);
}

tokens[pos] = NULL;
return (tokens);
}

/**
* launch - Launches a program as a child process
* @args: Array of arguments (program name and arguments)
*
* Description: Forks a child process and executes the program specified
* by args[0]. Uses execve to replace the child process image.
* Parent process waits for the child to finish.
*
* Return: Always returns 1 (for shell loop continuation)
*/
int launch(char **args)
{
pid_t pid;
int status;

pid = fork();

if (pid == 0)
{
if (execve(args[0], args, environ) == -1)
{
perror("execve");
exit(EXIT_FAILURE);
}
}
else if (pid < 0)
{
perror("fork");
}
else
{
do {
waitpid(pid, &status, WUNTRACED);
} while (!WIFEXITED(status) && !WIFSIGNALED(status));
}

return (1);
}

/**
* execute - Executes a command
* @args: Array of arguments parsed from user input
*
* Description: Takes a command and its arguments and passes them to launch.
* This is the main entry point for executing commands.
* Handles empty commands gracefully.
*
* Return: Status of the launched process (1 for normal execution)
*/
int execute(char **args)
{
if (args[0] == NULL)
return (1);

return (launch(args));
}
