#include <stdio.h>

int main(int argc, char const *argv[])
{
	int x;
	printf("Please enter an int: ");
	scanf("%i", &x);
	printf("You entered: %d\n", x);

	char w1[100];
	char w2[100];
	printf("Please enter a String: ");
	// scanf("%s %s", w1, w2);
	scanf("%s", w1);
	// printf("You entered: '%s %s'\n", w1, w2);
	printf("You entered: '%s'\n", w1);

	return 0;
}
