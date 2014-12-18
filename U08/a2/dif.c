/*
	Changed lines are marked with '//!' at the end.
*/

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>

#define EXIT_FAILURE -1

/*
	Gebe die restliche Zeile aus.
*/
char* printLine(FILE *fp, char *buffer, int buffersize)
{
	char *getF = fgets(buffer,buffersize,fp) ;

	while(!strchr(buffer,'\n') )
	{
		printf("%s",buffer); //! resolved uncontrolled format
		getF = fgets(buffer,buffersize,fp);
		if(getF == NULL)
		{
			return NULL;
		}
	}
	printf("%s",buffer); //! resolved uncontrolled format
	getF = fgets(buffer,buffersize,fp);
	return getF;
}

void printName(char* output, char* fname)
{
	strcpy(output,fname);
	strcat(output,": ");
	printf("%s\n",output); //! resolved uncontrolled format
}

FILE* openfile(char *fname)
{
	/*
		Überrpüfe die Dateien
		Existieren die Dateien?
		Haben wir Leserechte?
	*/
	FILE *fp;
	if (access(fname, F_OK) == -1) {
		fprintf(stderr,"File %s does not exist!\n", fname);
		return NULL;
	}
	fp = fopen(fname, "r");
	if (fp == NULL) {
		fprintf(stderr,"Cannot open %s for reading!\n", fname);
		return NULL;
	}
	return fp;
}
/*
	Berechnet naiv die unterschiedlichen Zeilen von zwei Dateien und gibt diese aus.
*/
int main()
{
	FILE *fp1, *fp2,*lastfp;
	char fname1[32], fname2[32], output[64];
	unsigned int wrong = 0;
	unsigned int line = 0;
	char *getF1, *getF2, *getLastFile;
	int size = 8;
	char buffer1[size], buffer2[size]; //! initialized with size variable

	/*Lese Dateiname ein.*/
	printf("Enter name of first file :");
	scanf("%s",fname1); //! replaced insecure gets with scanf
	printf("Enter name of second file:");
	scanf("%s",fname2); //! replaced insecure gets with scanf
	printf("\n\n");
	fp1 = openfile(fname1);
	fp2 = openfile(fname2);
	if (fp1 == NULL) {
		return EXIT_FAILURE;
	}
	else if (fp2 == NULL)
	{
		return EXIT_FAILURE;
	}

	/*
		Wir lesen die Dateien aus und vergleichen die Strings.
		Wir zaehlen die Anzahl der ungleichen Zeilen.
	*/
	getF1 = fgets(buffer1,size,fp1);
	getF2 = fgets(buffer2,size,fp2);
	getLastFile = NULL;
	while((getF1 != NULL) && (getF2!= NULL))
	{
		//! if we want to avoid to not match a last line that matches it's not last line counterpart, we need to remove the '\n' before comparing.
		char buffer1_c[size], buffer2_c[size]; //!
		strcpy(buffer1_c,buffer1); //!
		strcpy(buffer2_c,buffer2); //!
		strtok(buffer1_c,"\n"); //!
		strtok(buffer2_c,"\n"); //!
		if(strcmp(buffer1_c,buffer2_c) != 0) //!
		{
			wrong++;
			printf("Ungleiche Zeile.\n");
			/*
				Gebe die Zeile aus der ersten Datei aus.
			*/
			printName(output,fname1);
			printf("%s",buffer1);
			if(!strchr(buffer1,'\n'))
			{
				memset(buffer1,0,size); //! empty buffer before refilling
				getF1 = printLine(fp1,buffer1,size);
				if(feof(fp1)) printf("\n"); //! add linebreak at end of file
			}
			else
			{
				memset(buffer1,0,size); //! empty buffer before refilling
				getF1 = fgets(buffer1,size,fp1) ;
			}
			/*
				Gebe die Zeile aus der zweiten Datei aus.
			*/
			printName(output,fname2);
			printf("%s",buffer2); //! resolved uncontrolled format
			/*
				Überprüfe, ob die letzte Zeile ausgegeben werden muss.
			*/
			if(!strchr(buffer2,'\n'))
			{
				memset(buffer2,0,size); //! empty buffer before refilling
				getF2 = printLine(fp2,buffer2,size);
				if(feof(fp2)) printf("\n"); //! add linebreak at end of file
			}
			else
			{
				memset(buffer2,0,size); //! empty buffer before refilling
				getF2 = fgets(buffer2,size,fp2) ;
			}
		}
		else
		{
			memset(buffer1,0,size); //! empty buffer before refilling
			memset(buffer2,0,size); //! empty buffer before refilling
			getF1 = fgets(buffer1,size,fp1) ;
			getF2 = fgets(buffer2,size,fp2) ;
		}
		line++;
		printf("\t\t\tline = %3d | wrong = %3d \n",line,wrong);
	}
	/*
		Falls eine Datei früher komplett gelesen wurde.
		Wir geben den Dateinamen und den Rest aus dem Buffer aus.
	*/
	if(!(getF1 == NULL))
	{
		getLastFile = getF1;
		lastfp = fp1;
		printf("Verbleibende Zeilen.\n");
		printName(output,fname1);
		// printf(buffer1); //!
	}
	else if(!(getF2 == NULL))
	{
		getLastFile = getF2;
		lastfp = fp2;
		printf("Verbleibende Zeilen.\n");
		printName(output,fname2);
		strcpy(buffer1,buffer2); //!
		// printf(buffer2); //!
	}
	/*
		Wir lesen die restliche Datei aus.
	*/
	if(getLastFile != NULL)
	{
		// getLastFile = fgets(buffer1,size,lastfp); //!
		while(getLastFile !=NULL)
		{
			wrong++;
			printf("%s",buffer1); //! resolved uncontrolled format
			if(!strchr(buffer1,'\n')) //! distinguish between complete lines and incomplete lines
			{
				memset(buffer1,0,size); //! empty buffer before refilling
				getLastFile = printLine(lastfp,buffer1,size);
				if(feof(fp2)) printf("\n"); //! add linebreak at end of file
			}
			else
			{
				memset(buffer1,0,size); //! empty buffer before refilling
				getLastFile = fgets(buffer1,size,lastfp);
			}
			line++;
			printf("\t\t\tline = %3d | wrong = %3d \n",line,wrong);
		}
	}
	/*
		Sind die Datein verschieden?
	*/
	if(wrong > 0)
	{
		printf("\n\nDateien sind ungleich. Anzahl ungleicher Zeilen: %d\n",wrong);
	}
	else
	{
		printf("\n\nDateien sind gleich.\n");
	}
	fclose(fp1);
	fclose(fp2);
	return (0);
}
