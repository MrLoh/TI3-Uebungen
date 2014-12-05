#include <stdio.h>
#include <string.h>
#define MAX_LINE_SIZE 2048

void write_grepped(FILE* file, const char* key) {
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
	}
	else
		fputs("ERROR: no input file found\n", stderr);
}

int main(int argc, char const *argv[])
{
	if ( argc == 1 )
		fputs("ERROR: not enough input given\n", stderr);
	else
	{
		// char* key = argv[1];
		FILE *file;
		if ( argc == 2 )
		{
			// input in stdin
			write_grepped(stdin, argv[1]);
		}
		else
		{
			// filename in argv[2]
			file = fopen(argv[2],"r");
			write_grepped(file, argv[1]);
			fclose(file);
		}
	}

	return 0;
}
