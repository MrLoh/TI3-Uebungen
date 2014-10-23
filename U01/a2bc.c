#include <stdio.h>

double cel2fahr(double cel)
{
	return 1.8 * cel + 32.0;
}

int main()
{
	printf("   C     |   F \n");
	printf("---------|---------\n");
	for (double cel=-30; cel<=100; cel+=10)
	{
		printf("%6.1f   |  %5.1f \n", cel, cel2fahr(cel) );
	}

	return 0;
}
