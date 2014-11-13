// Tobias Lohse, Sven Klaus, Luisa Castano Jurado

#include <stdio.h>

char pw[10];
char txt[100];

int main(int argc, char const *argv[])
{
	printf("Enter a Passphrase: ")
	scanf("%s", pw);

	printf("Enter a Text: ")
	for (int i = 0; i < 100; ++i)
	{
		txt[i] = getchar()
	}

	return 0;
}

