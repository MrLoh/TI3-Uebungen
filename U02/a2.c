// Tobias Lohse, Sven Klaus, Luisa Castano Jurado

#include <stdio.h>

char pw[10];
char txt[100];

int main(int argc, char const *argv[])
{
	printf("Enter a Passphrase: ");
	// scanf("%s", pw);

	char text[100];
	fgets(text, 99, stdin);

	printf("%s", text);
	printf("%s", text);
	printf("%s", text);
	printf("%s", text);

	return 0;
}

char encode(char char, int index)

