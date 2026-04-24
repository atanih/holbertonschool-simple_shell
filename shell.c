#include "shell.h"

#define MAX_PATH_LEN 256

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
* find_command_in_path - Finds command in PATH
* @command: The command to find
*
* Description: Searches for the command in all directories listed in PATH.
* Uses static buffer to store the full path (no malloc).
*
* Return: Static buffer with full path, or NULL if not found
*/
char *find_command_in_path(char *command)
{
static char full_path[MAX_PATH_LEN];
char *path_env, *path_copy, *path_dir;
int i;

if (strchr(command, '/') != NULL)
{
if (access(command, F_OK) == 0)
return (command);
return (NULL);
}

path_env = NULL;
for (i = 0; environ[i] != NULL; i++)
{
if (strncmp(environ[i], "PATH=", 5) == 0)
{
path_env = environ[i] + 5;
break;
}
}

if (path_env == NULL || path_env[0] == '\0')
return (NULL);

path_copy = malloc(strlen(path_env) + 1);
if (path_copy == NULL)
return (NULL);
strcpy(path_copy, path_env);

path_dir = strtok(path_copy, ":");

while (path_dir != NULL)
{
sprintf(full_path, "%s/%s", path_dir, command);

if (access(full_path, F_OK) == 0)
{
free(path_copy);
return (full_path);
}

path_dir = strtok(NULL, ":");
}

free(path_copy);
return (NULL);
}

/**
* execute - Executes a command
* @args: Array of arguments parsed from user input
*
* Description: Takes a command and its arguments. Implements built-ins (exit, env)
* and searches for commands in PATH if needed.
*
* Return: Status of the launched process (1 for normal execution, 0 for exit)
*/
int execute(char **args)
{
char *full_path;
int i;

if (args[0] == NULL)
return (1);

if (strcmp(args[0], "exit") == 0)
return (0);

if (strcmp(args[0], "env") == 0)
{
i = 0;
while (environ[i] != NULL)
{
printf("%s\n", environ[i]);
i++;
}
return (1);
}

full_path = find_command_in_path(args[0]);

if (full_path == NULL)
{
fprintf(stderr, "./hsh: 1: %s: not found\n", args[0]);
return (127);
}

args[0] = full_path;
return (launch(args));
}
