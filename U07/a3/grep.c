#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void grep(FILE* file, const char* key)
{
	if ( file )
	{
		int linesize = 16;
		char line[linesize];
		int i = 0;
		int c;
		while ( (c = fgetc(file)) != EOF )
		{
			line[i] = c;
			i++;
			if ( c == '\n' )
			{
				// test if line contains key
				if ( strstr(line, key) != NULL )
					fputs(line, stdout);
				// clean up line to avoid leftovers in next
				memset(line, 0, i);
				i = 0;
			}
			if ( i+2 >= linesize)
			{
				// duplicate line size for too short line
				linesize *= 2;
				char temp[linesize];
				strcpy(temp, line);
				*line = *temp;
			}
		}
	}
	else
	{
		fputs("ERROR: no input file found\n", stderr);
		exit(EXIT_FAILURE);
	}
}

int main(int argc, char const *argv[])
{
	int exit_code = EXIT_FAILURE;

	FILE *file;
	if ( argc == 1 )
	{
		fputs("ERROR: not enough input given\n", stderr);
		exit(EXIT_FAILURE);
	}
	else if ( argc == 2 )
	{
		// input in stdin
		grep(stdin, argv[1]);
	}
	else
	{
		// filename in argv[2]
		file = fopen(argv[2],"r");
		grep(file, argv[1]);
		fclose(file);
	}

	return EXIT_SUCCESS;
}
