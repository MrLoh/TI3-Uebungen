// TI3 Uebung2, Tobias Lohse, Sven Klaus, Luisa Castano Jurado
#include <stdio.h>

int valueOf(char letter);
char getCharByValue(int num);
char encodeChar(char char1, char char2);
char decodeChar(char char1, char char2);
void encode(void);
void decode(void);
void readPass();

char userInput[10] = { 0 };
char pw[10] = { 0 };
char longPw[100] = { 0 };
char clearTxt[100] = { 0 };
char codeTxt[100] = { 0 };
char chiffreTxt[100] = { 0 };
char decodeTxt[100] = { 0 };
int idx = 0;

int main(int argc, char const *argv[])
{
	while (1) {
		printf("(D)ecode or (E)ncode Mode? \n");
		fgets(userInput, sizeof(userInput), stdin);

		if (userInput[1] == '\n' && userInput[0] == 'd') {
			decode();
			break;
		} else if (userInput[1] == '\n' && userInput[0] == 'e') {
			encode();
			break;
		} else {
			printf("Please enter 'd' or 'e' to continue.");
		}
	}
	return 0;
}

void decode(void) {
	printf("DECODE:\n");
	readPass();

	printf("Enter the encoded Text: \n");
	fgets(chiffreTxt, 99, stdin);

	// repeat pass for length of text
	idx = 0;
	for (int i = 0; i < 100; ++i) {
		if (chiffreTxt[i] && (chiffreTxt[i] != 10)) {
			do {
				if (pw[idx % 11]) {
					longPw[i] = pw[idx % 11];
				}
				idx++;
			} while (!pw[idx % 11]);
		}
	}

	// decode every char
	for (int i = 0; i < 100; ++i) {
		if (chiffreTxt[i] && (chiffreTxt[i] != 10)) {
			decodeTxt[i] = decodeChar(chiffreTxt[i], longPw[i]);
		}
	}

	printf("\n");
	printf("Passwort  : %s\n", pw);
	printf("Codewort  : %s\n", longPw);
	printf("Chiffre   : %s\n", chiffreTxt);
	printf("---------------------------------------------\n");
	printf("Decode    : %s\n", decodeTxt);
	printf("---------------------------------------------\n");

}

void encode(void) {
	printf("ENCODE:\n");
	readPass();

	printf("Enter the Text: \n");
	fgets(clearTxt, 99, stdin);

	// remove unused signs & spaces - convert to UpperCase
	idx = 0;
	for (int i = 0; i < 100; ++i) {
		char c = clearTxt[i];
		if (valueOf(c) >= 0) {
			codeTxt[idx] = getCharByValue(valueOf(c));
			idx++;
		}
	}

	// repeat pass for length of text
	idx = 0;
	for (int i = 0; i < 100; ++i) {
		if (codeTxt[i]) {
			do {
				if (pw[idx % 11]) {
					longPw[i] = pw[idx % 11];
				}
				idx++;
			} while (!pw[idx % 11]);
		}
	}

	//encode every char
	for (int i = 0; i < 100; ++i) {
		if (codeTxt[i]) {
			chiffreTxt[i] = encodeChar(codeTxt[i], longPw[i]);
		}
	}

	printf("\n");
	printf("Passwort  : %s\n", pw);
	printf("Klartext  : %s\n", clearTxt);
	printf("Codierung : %s\n", codeTxt);
	printf("Codewort  : %s\n", longPw);
	printf("---------------------------------------------\n");
	printf("Chiffre   : %s\n", chiffreTxt);
	printf("---------------------------------------------\n");
}

void readPass() {
	printf("Enter a Passphrase: \n");
	fgets(userInput, sizeof(userInput), stdin);

	idx = 0;
	for (int i = 0; i < 10; ++i) {
		if (userInput[i]) {
			if (getCharByValue(valueOf(userInput[i]))) {
				pw[idx] = getCharByValue(valueOf(userInput[i]));
				idx++;
			}
		}
	}
	printf("Acceptet Passphrase: %s\n", pw);
}

char encodeChar(char char1, char char2) {
	int charSum = valueOf(char1) + valueOf(char2);
	return getCharByValue(charSum % 36);
}

char decodeChar(char char1, char char2) {
	int charDiff = valueOf(char1) - valueOf(char2) + 36;
	return getCharByValue(charDiff % 36);
}

int valueOf(char letter) {
	int num = (int) letter;
	// Buchstaben A-Z
	if (num >= 65 && num <= 90) {
		return num - 65;
		// Buchstaben a-z
	} else if (num >= 97 && num <= 122) {
		return num - 97;
		//Zahlen 0-9
	} else if (num >= 48 && num <= 57) {
		return num - 22;
	} else {
		return -1;
	}
}

char getCharByValue(int num) {
	// Buchstaben A-Z
	if (num >= 0 && num <= 25) {
		return (char) num + 65;
		// Zahlen 0-9
	} else if (num >= 26 && num <= 35) {
		return (char) num + 22;
	} else {
		return (char) 0;
	}
}
