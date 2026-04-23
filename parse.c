#include "shell.h"

/**
 * hsh_strdup - duplicate a nul-terminated string
 * @s: source
 *
 * Return: new buffer or NULL
 */
static char *hsh_strdup(const char *s)
{
	char *d;
	size_t n;
	size_t j;

	if (s == NULL)
		return (NULL);
	n = 0;
	while (s[n] != '\0')
		n++;
	d = malloc(n + 1);
	if (d == NULL)
		return (NULL);
	for (j = 0; j <= n; j++)
		d[j] = s[j];
	return (d);
}

/**
 * line_to_argv - build argv from a single line; tokens split on space/tab
 * @line: nul-terminated line
 * @ac: out argc
 * @out_linebuf: backing buffer; free with free_line_args
 *
 * Return: NULL if empty line or failure
 */
char **line_to_argv(const char *line, int *ac, char **out_linebuf)
{
	char *buf;
	char **av;
	int i;
	int w;
	int n;

	if (ac)
		*ac = 0;
	if (out_linebuf)
		*out_linebuf = NULL;
	if (line == NULL)
		return (NULL);
	buf = hsh_strdup(line);
	if (buf == NULL)
		return (NULL);
	i = 0;
	n = 0;
	while (buf[i] != '\0')
	{
		while (buf[i] == ' ' || buf[i] == '\t')
			i++;
		if (buf[i] == '\0')
			break;
		n++;
		while (buf[i] != '\0' && buf[i] != ' ' && buf[i] != '\t')
			i++;
	}
	if (n == 0)
	{
		free(buf);
		return (NULL);
	}
	av = malloc((size_t)(n + 1) * sizeof(*av));
	if (av == NULL)
	{
		free(buf);
		return (NULL);
	}
	i = 0;
	w = 0;
	while (1)
	{
		while (buf[i] == ' ' || buf[i] == '\t')
			i++;
		if (buf[i] == '\0')
			break;
		av[w] = &buf[i];
		w++;
		while (buf[i] != '\0' && buf[i] != ' ' && buf[i] != '\t')
			i++;
		if (buf[i] == '\0')
			break;
		buf[i] = '\0';
		i++;
	}
	av[w] = NULL;
	if (ac)
		*ac = w;
	if (out_linebuf)
		*out_linebuf = buf;
	return (av);
}

/**
 * free_line_args - free argv and backing string buffer
 * @argv: table from line_to_argv
 * @ac: number of arguments
 * @linebuf: pointer returned in out_linebuf
 */
void free_line_args(char **argv, int ac, char *linebuf)
{
	(void)ac;

	if (linebuf)
		free(linebuf);
	if (argv)
		free(argv);
}
