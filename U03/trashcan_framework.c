/**
 * File: trashcan.c
 * TI 2 - Betriebs- und Kommunikationssysteme
 * Freie Universitaet Berlin
 */
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFSIZE			1024
#define PATHSIZE		256
#define TRASHFOLDER	".ti3_trash"


char copy_buffer[BUFSIZE];


/* 'copy' kopiert den Dateiinhalt einer Datei namens
 * "source". Eine Kopie target wird nur erzeugt, wenn
 * eine Datei "target" noch nicht existiert.
 * Die Zugriffsrechte werden nicht kopiert, sondern
 * auf "rwx --- ---" gesetzt.
 */
int copy(char *source, char *target)
{
	ssize_t readen;
	ssize_t written;
	int h_target;
	int h_source;

	if ( h_source = open(source, O_RDONLY), h_source == -1 )
		return -1;
	if ( h_target = open(target, O_WRONLY|O_CREAT|O_EXCL, 0644), h_target == -1 )
		return -2;
	while ( readen = read(h_source, copy_buffer, BUFSIZE), readen > 0 )
	{
		written = write(h_target, copy_buffer, readen);
		if ( readen-written != 0 )
			return -3;
	}
	close(h_source);
	close(h_target);
	return 0;
}


char* getpath(char *foldername, char *filename)
{
	ssize_t length = strlen(foldername)+strlen(filename)+2;
	char *path = (char*) malloc(length);
	strcpy(path, foldername);
	strcat(path, "/");
	strcat(path, filename);

	return path;
}


char parse_command(char *command)
{
	return command[1];
}


/* erzeugt einen Ordner foldername */
int setup_trashcan(char *foldername)
{
	return mkdir(foldername, 0755);
}

/* führt trashcan -p[ut] filename aus */
int put_file(char *foldername, char *filename)
{
	char *source = filename;
	char *target = getpath(foldername, filename);
	int err;

	if ( err = copy(source, target), err < 0 )
	{
		switch (err) {
			case -1: return -1;
			case -2: return -2;
			case -3: return -2;
		}
	}
	if ( remove(source) == -1)
		return -3;
	return 0;
}


/* führt trashcan -g[et] filename aus */
int get_file(char *foldername, char *filename)
{
	char *source = getpath(foldername, filename);
	char *target = filename;
	int err;

	if ( err = copy(source, target), err < 0 )
	{
		switch (err) {
			case -1: return -1;
			case -2: return -2;
			case -3: return -2;
		}
	}
	if ( remove(source) == -1)
		return -3;
	return 0;
}

/* führt trashcan -r[emove] filename aus */
int remove_file(char *foldername, char *filename)
{
	char *path = getpath(foldername, filename);
	if ( remove(path) == -1 )
		return -1;
	return 0;
}


int main(int argc, char *argv[])
{
	if (argc == 1) {
		printf("...not enough arguments!\n");
		return EXIT_FAILURE;
	} else {
		setup_trashcan(TRASHFOLDER);
		char command = parse_command(argv[1]);
		switch (command) {
			case 'p':
				if (argc != 3) {
					printf("...not enough arguments!\n");
					return EXIT_FAILURE;
				}
				int p_error = put_file(TRASHFOLDER, argv[2]);
				switch (p_error) {
					case -1:
						printf("...source file not found!\n");
						break;
					case -2:
						printf("...trash file was not created!\n");
						break;
					case -3:
						printf("...source file could not be removed!\n");
					default:
						break;
				}
				break;
			case 'g':
				if (argc != 3) {
					printf("...not enough arguments!\n");
					return EXIT_FAILURE;
				}
				int g_error = get_file(TRASHFOLDER, argv[2]);
				switch (g_error) {
					case -1:
						printf("...trash file not found!\n");
						break;
					case -2:
						printf("...restore file was not created!\n");
						break;
					case -3:
						printf("...trash file could not be removed!\n");
					default:
						break;
				}
				break;
			case 'r':
				if (argc != 3) {
					printf("...not enough arguments!\n");
					return EXIT_FAILURE;
				}
				int r_error = remove_file(TRASHFOLDER, argv[2]);
				if (r_error)
					printf("...trash file could not be removed!\n");
				break;
			default :
				printf("...unknown command!\n");
				return EXIT_FAILURE;
		}
	}
	return EXIT_SUCCESS;
}
