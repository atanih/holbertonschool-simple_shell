#include "shell.h"

static char *path_join(const char *dir, const char *cmd);

/**
 * hsh_strdup - duplicate a string
 * @s: source
 *
 * Return: new buffer
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

static const char *get_path_key(char **envp)
{
	int i;

	if (envp == NULL)
		return (NULL);
	for (i = 0; envp[i] != NULL; i++)
	{
		if (strncmp(envp[i], "PATH=", 5) == 0)
			return (envp[i] + 5);
	}
	return (NULL);
}

/**
 * path_join - concatenate dir, '/', cmd
 * @dir: directory, non-NULL
 * @cmd: command name
 *
 * Return: full path
 */
static char *path_join(const char *dir, const char *cmd)
{
	const char *d;
	const char *c;
	char *o;
	unsigned int a;
	unsigned int b;
	unsigned int p;

	if (dir == NULL || cmd == NULL)
		return (NULL);
	d = dir;
	c = cmd;
	a = 0U;
	b = 0U;
	while (d[a] != '\0')
		a++;
	while (c[b] != '\0')
		b++;
	o = malloc((size_t)(a + b + 2U));
	if (o == NULL)
		return (NULL);
	p = 0U;
	d = dir;
	c = cmd;
	while (*d)
	{
		o[p] = *d;
		p++;
		d++;
	}
	if (p == 0U || o[p - 1U] != '/')
	{
		o[p] = '/';
		p++;
	}
	while (*c)
	{
		o[p] = *c;
		p++;
		c++;
	}
	o[p] = '\0';
	return (o);
}

/**
 * path_resolve - locate executable, no fork; caller may fork
 * @cmd: first argument
 * @envp: environment
 *
 * Return: malloc'd path, or NULL
 */
char *path_resolve(const char *cmd, char **envp)
{
	const char *pkey;
	char *pcpy;
	char *tok;
	char *full;
	int k;

	if (cmd == NULL || *cmd == '\0')
		return (NULL);
	k = 0;
	for (; cmd[k] != '\0'; k++)
	{
		if (cmd[k] == '/')
		{
			if (access(cmd, X_OK) == 0)
				return (hsh_strdup(cmd));
			return (NULL);
		}
	}
	pkey = get_path_key(envp);
	if (pkey == NULL)
		return (NULL);
	pcpy = hsh_strdup(pkey);
	if (pcpy == NULL)
		return (NULL);
	for (tok = strtok(pcpy, ":"); tok != NULL; tok = strtok(NULL, ":"))
	{
		if (tok[0] == '\0')
			tok = (char *)".";
		full = path_join(tok, cmd);
		if (full == NULL)
		{
			free(pcpy);
			return (NULL);
		}
		if (access(full, X_OK) == 0)
		{
			free(pcpy);
			return (full);
		}
		free(full);
	}
	free(pcpy);
	return (NULL);
}
