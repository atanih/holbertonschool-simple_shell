#include "shell.h"

/**
 * hsh_loop - read lines, run builtins or children
 * @argv0: shell's argv[0] for errors
 * @envp: environment
 *
 * Return: 0
 */
int hsh_loop(const char *argv0, char **envp)
{
	char *line;
	size_t cap;
	ssize_t n;
	int line_no;
	int inter;
	int ac;
	char *linebuf;
	char **av;
	int should_ex;
	int hb;
	char *rpath;
	int last;

	line = NULL;
	cap = 0U;
	line_no = 1;
	inter = isatty(STDIN_FILENO);
	while (1)
	{
		if (inter)
		{
			write(STDOUT_FILENO, "($) ", 4);
			fflush(stdout);
		}
		n = getline(&line, &cap, stdin);
		if (n == (ssize_t)-1)
		{
			if (line)
				free(line);
			if (inter)
				write(STDOUT_FILENO, "\n", 1);
			return (0);
		}
		last = (int)(n) - 1;
		if (last >= 0 && line != NULL && line[last] == '\n')
			line[last] = '\0';
		av = line_to_argv(line, &ac, &linebuf);
		if (av == NULL)
		{
			free(line);
			line = NULL;
			line_no++;
			continue;
		}
		hb = handle_builtin(ac, av, envp, &should_ex, NULL);
		if (hb)
		{
			if (should_ex)
			{
				free_line_args(av, ac, linebuf);
				free(line);
				return (0);
			}
			free_line_args(av, ac, linebuf);
			free(line);
			line = NULL;
			line_no++;
			continue;
		}
		rpath = path_resolve(av[0], envp);
		if (rpath == NULL)
		{
			fprintf(stderr, "%s: %d: %s: not found\n", argv0, line_no, av[0]);
			free_line_args(av, ac, linebuf);
			free(line);
			line = NULL;
			line_no++;
			continue;
		}
		launch_child(rpath, av, envp);
		free(rpath);
		free_line_args(av, ac, linebuf);
		free(line);
		line = NULL;
		line_no++;
	}
}
