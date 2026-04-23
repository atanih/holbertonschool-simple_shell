#include "shell.h"

/**
 * main - entry point: run simple shell with inherited environment
 * @ac: arg count
 * @argv: program name in argv[0] for error label parity
 * @envp: process environment
 *
 * Return: hsh_loop status
 */
int main(int ac, char **argv, char **envp)
{
	(void)ac;

	if (argv == NULL || argv[0] == NULL)
		return (1);

	return (hsh_loop(argv[0], envp));
}
