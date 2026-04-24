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
* find_command_in_path - Finds command in PATH
* @command: The command to find
*
* Description: Searches for the command in all directories listed in PATH.
* Returns the full path if found, NULL otherwise.
*
* Return: Full path to command (malloc'd), or NULL if not found
*/
char *find_command_in_path(char *command)
{
char *path_env, *path_copy, *path_dir, *full_path;
int i, cmd_len, dir_len;

if (strchr(command, '/') != NULL)
{
if (access(command, F_OK) == 0)
{
full_path = malloc(strlen(command) + 1);
if (full_path == NULL)
return (NULL);
strcpy(full_path, command);
return (full_path);
}
return (NULL);
}

for (i = 0; environ[i] != NULL; i++)
{
if (strncmp(environ[i], "PATH=", 5) == 0)
{
path_env = environ[i] + 5;
break;
}
}

if (environ[i] == NULL)
return (NULL);

path_copy = malloc(strlen(path_env) + 1);
if (path_copy == NULL)
return (NULL);
strcpy(path_copy, path_env);

cmd_len = strlen(command);
path_dir = strtok(path_copy, ":");

while (path_dir != NULL)
{
dir_len = strlen(path_dir);
full_path = malloc(dir_len + cmd_len + 2);

if (full_path == NULL)
{
free(path_copy);
return (NULL);
}

sprintf(full_path, "%s/%s", path_dir, command);

if (access(full_path, F_OK) == 0)
{
free(path_copy);
return (full_path);
}

free(full_path);
path_dir = strtok(NULL, ":");
}

free(path_copy);
return (NULL);
}

/**
* execute - Executes a command
* @args: Array of arguments parsed from user input
*
* Description: Takes a command and its arguments. Searches for the command
* in PATH if needed. If command is found, passes it to launch.
* If command is not found, prints error and returns without forking.
*
* Return: Status of the launched process (1 for normal execution)
*/
int execute(char **args)
{
char *full_path;

if (args[0] == NULL)
return (1);

full_path = find_command_in_path(args[0]);

if (full_path == NULL)
{
fprintf(stderr, "./hsh: 1: %s: not found\n", args[0]);
return (1);
}

args[0] = full_path;
launch(args);

return (1);
}
