#include <stdio.h>
#include <stdlib.h>


/*Implementierung einer Prozessverwaltung:

Implementieren Sie die durch das Framework vorgegebene Prozessverwaltung,
die eine Menge von Prozessen aus der Datei prcs.dat auslesen,und diese
in eine doppelt verkettete Liste einfuegen soll.

Nun soll Prozess um Prozess der Reihenfolge ihrer ids entsprechend
aus der Liste entfernt werden,wobei die uebrig gebliebenen Prozesse in jeder
Iteration ausgegeben werden.

Die Prozesse sind in der Datei prcs.dat Zeile fuer Zeile in der Form:
ProzessId,Ankunftszeit,Ausfuehrungszeit
gespeichert.

*/

struct pData //Daten der doppelt verketteten Liste
{
	int pId;//ProzessID
	int aTime;//Ankunftszeit
	int sTime;//Ausfuehrungszeit
	struct pData *next;
	struct pData *prev;
};

//Um Tipparbeit zu sparen
typedef struct pData PROCESS;
typedef PROCESS *LINK;
int SIZE;

LINK findNextProcess(LINK head);

void addProcess(LINK head,LINK newnode);
void deleteProcess(LINK current);
void printPr(LINK current);//Methode zur Ausgabe eines Prozesses
void printList(LINK head);//Alle Prozesse der Liste ausgeben
void readProcesses(LINK head);//Einlesen der Prozesse

int main(void)
{
	LINK head = malloc(sizeof(PROCESS));
	head->next = head;
	head->prev = head;
	readProcesses(head);
	while ( head->next != head )
	{
		printList(head);
		LINK next = findNextProcess(head);
		deleteProcess(next);
	}
	printList(head);

	return 0;
}

/*==========================================================================
Implementierung der Methoden*/


//Liest die Prozesse ein und erstellt die Liste
void readProcesses(LINK head)
{
	FILE *file = fopen("prcs.dat.txt", "r");
	if (file)
	{
		while( !feof(file) )
		{
			LINK newnode = malloc(sizeof(PROCESS));
			int id,at,st;
 			char c1,c2;
			fscanf(file, "%d %c %d %c %d", &id, &c1, &at, &c2, &st);
			newnode->pId = id;
			newnode->aTime = at;
			newnode->sTime = st;
			addProcess(head, newnode);
		}
		fclose(file);
	}
}

//Einen Prozess an das Ende der Liste (also vor head) einfuegen
void addProcess(LINK head,LINK newnode)
{
	newnode->next = head;
	newnode->prev = head->prev;
	head->prev->next = newnode;
	head->prev = newnode;
	SIZE++;
}

//Loeschen des angegebenen Knotens
void deleteProcess(LINK current)
{
	current->prev->next = current->next;
	current->next->prev = current->prev;
	SIZE--;
	free(current);
}

/*gibt den Prozess mit der kleinsten Id aus der Liste zurueck*/
LINK findNextProcess(LINK head)
{
	LINK curr = head->next;
	LINK min = curr;
	while ( curr != head )
	{
		if ( curr->pId < min->pId )
			min = curr;
		curr = curr->next;
	}
	return min;
}


/*Ausgabemethoden fuer die Prozesse und die gesamte Liste*/
void printPr(LINK current)
{
	printf("\n pId:%d arrival time:%d service time:%d\n ",current->pId,current->aTime,current->sTime);
}

void printList(LINK head)
{
	printf("\n\n Current QUEUE of PROCESSES ---------------------------------------\n\n");
	LINK curr = head->next;
	while ( curr != head )
	{
		printPr(curr);
		curr = curr->next;
	}
}


