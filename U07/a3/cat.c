#include <stdio.h>
#include <stdlib.h>

void cat(FILE* file)
{
	if ( file )
	{
		int c;
		while ( (c = fgetc(file)) != EOF )
			fputc(c, stdout);
	}
	else
	{
		fputs("ERROR: no input given\n", stderr);
		exit(EXIT_FAILURE);
	}
}

int main(int argc, char const *argv[])
{
	if ( argc == 1 )
	{
		// input in stdin
		cat(stdin);
	}
	else
	{
		// filename in argv[1]
		FILE *file = fopen(argv[1],"r");
		cat(file);
		fclose(file);
	}

	return EXIT_SUCCESS;
}
