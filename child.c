#include "shell.h"

/**
 * launch_child - fork, execve in child, wait in parent
 * @fullpath: resolved path to the binary
 * @argv: argument vector including program name
 * @envp: environment
 *
 * Return: 0 on success (child ran), -1 on fork failure, child exit handled
 */
int launch_child(const char *fullpath, char **argv, char **envp)
{
	pid_t p;
	int st;

	if (fullpath == NULL || argv == NULL)
		return (-1);
	p = fork();
	if (p < 0)
	{
		perror("fork");
		return (-1);
	}
	if (p == 0)
	{
		execve(fullpath, argv, envp);
		perror("execve");
		_exit(127);
	}
	if (waitpid(p, &st, 0) < 0)
	{
		perror("waitpid");
		return (-1);
	}
	return (0);
}
