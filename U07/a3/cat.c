#include <stdio.h>
#define BUFF_SIZE 100

void write_out(FILE* file) {
	while ( !feof(file) )
	{
		char buffer[BUFF_SIZE];
		size_t read = fread(buffer, 1, BUFF_SIZE, file);
		size_t written = fwrite(buffer, 1, read, stdout);
	}
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
		// input in argv[1]
		FILE *file = fopen(argv[1],"r");
		write_out(file);
	}
	return 0;
}
