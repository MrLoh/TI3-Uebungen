#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LINE_SIZE 2048

int write_grepped(FILE* file, const char* key)
{
	if ( file )
	{
		while ( !feof(file) )
		{
			char buffer[MAX_LINE_SIZE];
			fgets(buffer, MAX_LINE_SIZE, file);
			if ( strstr(buffer, key) != NULL )
				fputs(buffer, stdout);
			memset(buffer, 0, MAX_LINE_SIZE);
		}
		return EXIT_SUCCESS;
	}
	else
	{
		fputs("ERROR: no input file found\n", stderr);
		return EXIT_FAILURE;
	}
}

int main(int argc, char const *argv[])
{
	int exit_code = EXIT_FAILURE;

	FILE *file;
	if ( argc == 1 )
		fputs("ERROR: not enough input given\n", stderr);
	else if ( argc == 2 )
	{
		// input in stdin
		exit_code = write_grepped(stdin, argv[1]);
	}
	else
	{
		// filename in argv[2]
		file = fopen(argv[2],"r");
		exit_code = write_grepped(file, argv[1]);
		fclose(file);
	}

	return exit_code;
}
