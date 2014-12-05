#include <stdio.h>
#include <stdlib.h>
#define BUFF_SIZE 512

int write_out(FILE* file)
{
	if ( file )
	{
		while ( !feof(file) )
		{
			char buffer[BUFF_SIZE];
			size_t read = fread(buffer, 1, BUFF_SIZE, file);
			fwrite(buffer, 1, read, stdout);
		}
		return EXIT_SUCCESS;
	}
	else
	{
		fputs("ERROR: no input given\n", stderr);
		return EXIT_FAILURE;
	}

}

int main(int argc, char const *argv[])
{
	int exit_code = EXIT_FAILURE;

	if ( argc == 1 )
	{
		// input in stdin
		exit_code = write_out(stdin);
	}
	else
	{
		// filename in argv[1]
		FILE *file = fopen(argv[1],"r");
		exit_code = write_out(file);
		fclose(file);
	}

	return exit_code;
}
