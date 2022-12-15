#include "monty.h"

global_t vg;

/**
 * free_vg - frees the global variables.
 */
void free_vg()
{
	free_dlistint(vg.head);
	free(vg.buffer);
	fclose(vg.fd);
}

/**
 * _start_vg - initalizes the global variables.
 * @fd: file descriptor.
 */
void start_vg(FILE *fd)
{
	vg.lifo = 1;
	vg.cont = 1;
	vg.arg = NULL;
	vg.head = NULL;
	vg.fd = fd;
	vg.buffer = NULL;
}

/**
 * check_input - checks existance of the file and if it can be opened.
 * @argc: argument count
 * @argv: argument vector
 *
 * Return: file struct
 */
FILE *check_input(int argc, char *argv[])
{
	FILE *fd;
	if (argc == 1 || argc > 2)
	{
		dprintf(2, "USAGE: monty file\n");
		exit(EXIT_FAILURE);
	}

	fd = fopen(argv[1], "r");
	if (fd == NULL)
	{
		dprintf(2, "Error: Can't open file %s\n", argv[1]);
		exit(EXIT_FAILURE);
	}

	return (fd);
}

/**
 * main - entry point to program.
 * @argc: argument count
 * @argv: argument vector
 * Return: 0 on success
 */
int main(int argc, char *argv[])
{
	void (*f)(stack_t **stack, unsigned int line_number);
	FILE *fd;
	size_t size = 256;
	ssize_t nlines = 0;
	char *lines[2] = {NULL, NULL};

	fd = check_input(argc, argv);
	start_vg(fd);
	nlines = getline(&vg.buffer, &size, fd);
	while (nlines != -1)
	{
		lines[0] = _strtoky(vg.buffer, " \t\n");
		if (lines[0] && lines[0][0] != '#')
		{
			f = get_opcodes(lines[0]);
			if (!f)
			{
				dprintf(2, "L%u: ", vg.cont);
				dprintf(2, "unknown instruction %s\n", lines[0]);
				free_vg();
				exit(EXIT_FAILURE);
			}
			vg.arg = _strtoky(NULL, " \t\n");
			f(&vg.head, vg.cont);
		}
		nlines = getline(&vg.buffer, &size, fd);
		vg.cont++;
	}

	free_vg();
	return (0);
}
