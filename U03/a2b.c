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
	mode_t mode = S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH;
	ssize_t readen;
	ssize_t written;
	int h_target;
	int h_source;

	if ( h_target = open(target, O_WRONLY|O_CREAT|O_EXCL, mode), h_target == -1 )
	{
		printf("ERROR: target file already exists.\n");
		return -1;
	}
	if ( h_source = open(source, O_RDONLY), h_source == -1 )
	{
		printf("ERROR: source file doesn't exists.\n");
		return -1;
	}
	while ( readen = read(h_source, copy_buffer, BUFSIZE), readen > 0 )
	{
		written = write(h_target, copy_buffer, readen);
		if ( readen-written != 0 )
		{
			printf("ERROR: write failed, please try again.\n");
			return -1;
		}
	}
	close(h_source);
	close(h_target);
	return 0;
}


int main(int argc, char const *argv[])
{
	copy("test.txt", "test_cp.txt");
	return 0;
}
