#include "shell.h"

/**
 * hsh_printenv - print environment
 * @envp: environment pointer array
 */
static void hsh_printenv(char **envp)
{
	int i;

	if (envp == NULL)
		return;
	for (i = 0; envp[i] != NULL; i++)
		printf("%s\n", envp[i]);
}

/**
 * handle_builtin - parent-side built-ins, no execve
 * @ac: argument count
 * @av: argv
 * @envp: environment
 * @should_exit: set to 1 for exit
 * @exit_st: return status for the shell
 *
 * Return: 0 if not a builtin, 1 if handled
 */
int handle_builtin(int ac, char **av, char **envp, int *should_exit, int *exit_st)
{
	if (ac < 1 || av == NULL || av[0] == NULL)
		return (0);
	if (should_exit)
		*should_exit = 0;
	if (exit_st)
		*exit_st = 0;
	if (strcmp(av[0], "exit") == 0)
	{
		if (should_exit)
			*should_exit = 1;
		if (exit_st)
			*exit_st = 0;
		return (1);
	}
	if (strcmp(av[0], "env") == 0)
	{
		hsh_printenv(envp);
		return (1);
	}
	return (0);
}
