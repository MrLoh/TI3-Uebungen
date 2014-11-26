#include "mm.h"
#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[])
{
	status();

	int *a = (int*)my_malloc(sizeof(int));
	*a = 5;
	printf(">> int *a (int*)my_malloc(sizeof(int))\n");
	printf(">> *a = %d\n\n",*a);
	status();

	char* b = (char*)my_malloc(16);
	b[0] = 'c';
	b[1] = 'o';
	b[2] = 'o';
	b[3] = 'l';
	b[4] = '\0';
	printf(">> char* b = (char*)my_malloc(256)\n");
	printf(">> *b = '%s'\n\n",b);
	status();


	char* c = (char*)my_malloc(256);
	printf(">> char* c = (char*)my_malloc(256)\n\n");
	status();

	char* d = (char*)my_malloc(9728);
	d[9727] = '\0';
	printf(">> char* d = (char*)my_malloc(9728)\n");
	printf(">> d[9959] = '%c' \n\n", d[9727]);
	status();

	my_free((void*)b);
	printf(">> my_free(b)\n\n");
	status();

	my_free((void*) d);
	printf(">> my_free(d)\n\n");
	status();

	char *e = (char*)my_malloc(80);
	printf(">> char *e = (char*)my_malloc(80)\n\n");
	status();

	my_free((void*)c);
	printf(">> my_free(c)\n\n");
	status();

	my_free((void*)a);
	printf(">> my_free(a)\n\n");
	status();

	my_free((void*)e);
	printf(">> my_free(e)\n\n");
	status();

	char buffer[3];
	printf("Press any key to continue...");
	fgets(buffer, 2, stdin);

}

