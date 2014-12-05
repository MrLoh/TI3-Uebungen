#include <stdio.h>
#define BUFF_SIZE 512

void write_out(FILE* file) {
	if ( file )
	{
		while ( !feof(file) )
		{
			char buffer[BUFF_SIZE];
			size_t read = fread(buffer, 1, BUFF_SIZE, file);
			fwrite(buffer, 1, read, stdout);
		}
	}
	else
		fputs("ERROR: no input given\n", stderr);
}

int main(int argc, char const *argv[])
{
	if ( argc == 1 )
	{
		// input in stdin
		write_out(stdin);
	}
	else
	{
		// filename in argv[1]
		FILE *file = fopen(argv[1],"r");
		write_out(file);
		fclose(file);
	}
	return 0;
}
