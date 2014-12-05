#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

void wc(FILE* file)
{
	if ( file )
	{
		int lines = 1;
		int words = 0;
		int chars = 0;
		int lastwasspace = 1;
		int c;
		while ( (c = fgetc(file)) != EOF )
		{
			chars++;
			if ( c == '\n' )
				lines++;
			if ( isspace(c) && !lastwasspace )
				words++;
			lastwasspace = isspace(c);
		}
		printf("Lines: %3d\n", lines);
		printf("Words: %3d\n", words);
		printf("Chars: %3d\n", chars);
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
		wc(stdin);
	}
	else
	{
		// filename in argv[1]
		FILE *file = fopen(argv[1],"r");
		wc(file);
		fclose(file);
	}

	return EXIT_SUCCESS;
}
