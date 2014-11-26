#include "mm.h"
#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[])
{
	status();

	printf(">> int *a (int*)my_malloc(sizeof(int))\n");
	int *a = (int*)my_malloc(sizeof(int));
	printf(">> *a = %d\n\n",*a);
	*a = 5;
	status();

	printf(">> char* b = (char*)my_malloc(256)\n");
	char* b = (char*)my_malloc(16);
	printf(">> *b = '%s'\n\n",b);
	b[0] = 'c'; b[1] = 'o'; b[2] = 'o'; b[3] = 'l'; b[4] = '\0';
	status();

	printf(">> char* c = (char*)my_malloc(256)\n\n");
	char* c = (char*)my_malloc(256);
	status();

	printf(">> char* d = (char*)my_malloc(9728)\n");
	char* d = (char*)my_malloc(9728);
	printf(">> d[9959] = '%c' \n\n", d[9727]);
	d[9727] = '\0';
	status();

	printf(">> my_free(b)\n\n");
	my_free((void*)b);
	status();

	printf(">> my_free(d)\n\n");
	my_free((void*) d);
	status();

	printf(">> char *e = (char*)my_malloc(80)\n\n");
	char *e = (char*)my_malloc(80);
	status();

	printf(">> my_free(c)\n\n");
	my_free((void*)c);
	status();

	printf(">> my_free(a)\n\n");
	my_free((void*)a);
	status();

	printf(">> my_free(e)\n\n");
	my_free((void*)e);
	status();

	char buffer[3];
	printf("Press any key to continue...");
	fgets(buffer, 2, stdin);

}

