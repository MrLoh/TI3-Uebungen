#include "mm.h"
#include <stdio.h>

//Groesse des Speichers, den die Speicherverwaltung verwalten soll
#define memorySize 10240

//Zustand von einem Speicherblock
enum memBlockState{not_allocated=0, allocated=1};
//zur spaeteren Ausgabe..
char boolStr[2][6] = {{"FALSE"},{"TRUE"}};

//ein Speicherblock
typedef struct _memoryblock {
    void* data;                         //Zeiger auf die Daten
    int dataLength;                     //Laenge des Blocks
    enum memBlockState state;           //Wird der Block benutzt?
    struct _memoryblock* nextBlock;     //Zeiger auf naechsten Speicherblock
} memoryBlock;

#define memoryBlockHeaderSize (int)sizeof(memoryBlock)

//definition von "privaten" funktionen
void mergeFreeBlocks();
memoryBlock* splitBlock(memoryBlock* block, int byteCount);

//Erster Speicherblock in der Liste
memoryBlock* head;
//Unser Speicher, den wir fuer die Daten und deren Verwaltung benutzen koennen
char memory[memorySize];

//Ist unsere Speicherverwaltung initialisiert und bereit benutzt zu werden?
int b_initialized = 0;

//Initialisiere die Datenstruktur(en) fuer unsere Speicherverwaltung
void initialize()
{
    if(!b_initialized)
    {
        head = (memoryBlock*)memory;
        //Initialisiere head in unserem Speicher <memory>
        //Unser gesamter Speicher <memory> soll als ein memoryBlock interpretiert werden
        //Wir müssen beachten, dass der memoryBlock head auch im Speicher <memory>
        //abgespeichert wird und somit der head auch Speicher der Größe
        //memoryBlockHeaderSize zur Verwaltung bennötigt.
        //Anfangs ist der restliche Speicher verfügbar und wird dem head als data zugeordnet.
        // Data beginnt somit nicht am Anfang von <memory>.

        b_initialized = 1;
        //INITIALISIERUNG VON HEAD
        head->dataLength = memorySize-memoryBlockHeaderSize+1;
        head->data = (void*)((char*)head+memoryBlockHeaderSize);
        head->state = not_allocated;
        head->nextBlock = NULL;
    }
}

//liefert den ingesamt freien Speicherplatz zurueck
int get_free_space()
{
    if(!b_initialized)
        return -1;
    int count = 0;
    memoryBlock* block = head;
    while(block != NULL)
    {
        if(block->state == not_allocated)
            count = count + block->dataLength;
        block = block->nextBlock;
    }
    return(count);
}

//versucht einen Speicherbereich mit <byteCount> vielen Bytes zu reservieren
//Falls dies nicht gelingt wird ein NULL (0) Pointer zurueckgeliefert
void* my_malloc(int byteCount)
{
    if(!b_initialized)
        initialize();
    //Wenn der insgesamt verfuegbare Speicherplatz kleiner ist
    //als der angeforderte, koennen wir gleich aufhoeren!
    if(byteCount > get_free_space())
        return(NULL);

    //SUCHE NACH EINEM GEEIGNETEN FREIEN SPEICHERBLOCK, MIT MEHR ALS <byteCount>
    //VIELEN BYTES
    memoryBlock* block = head;
    while(block != NULL)
    {
        if(block->dataLength > byteCount && block->state == not_allocated)
            goto allocate_memory;
        block = block->nextBlock;
    }
    // FALLS ES KEIN PASSENDES ELEMENT GIBT, GEBEN WIR NULL ZURÜCK.
    return(NULL);

    //Der Knoten block hat genuegend Speicherplatz
    allocate_memory:
        // UNTERTEILUNG DIESES BLOCKS, SO DASS NICHT UNNÖTIG VIEL SPEICHERPLATZ VERBRAUCHT WIRD
        // UND RÜCKGABE DES ZEIGERS AUF DEN ZU BENUTZENDEN SPEICHERBEREICH
        return splitBlock(block, byteCount)->data;
}

//Sofern moeglich teilt die Funktion splitBlock einen Block in 2 Bloecke,
//so dass einer der beiden Bloecke <byteCount> viel Speicherplatz enthaelt.
//Die Funktion liefert denjenigen Block zurueck, der <byteCount> viel Speicherplatz
//bereitstellt und passt die verkette Liste an.
memoryBlock* splitBlock(memoryBlock* block, int byteCount)
{
    // Hinweis: Es ist sinnvoll, wenn <block> die Laenge byteCount hat und
    // der Nachfolger von <block> die Restlaenge umfasst.

    //BERECHNE DIE GROESSE DES NEUEN UND ALTEN BLOCKS
    int remainingDataLength = block->dataLength - byteCount;

    //FALLS EIN WEITERER SPEICHERBLOCK IN DEN ALTEN PASST,
    //ERZEUGEN WIR EINEN NEUEN BLOCK, AENHLICH ZU HEAD AM ANFANG
    if(remainingDataLength > memoryBlockHeaderSize)
    {
        memoryBlock *new_block = (void*)((char*)block->data+byteCount);
        new_block->dataLength = remainingDataLength-memoryBlockHeaderSize;
        new_block->data = (void*)((char*)new_block+memoryBlockHeaderSize);
        new_block->state = not_allocated;
        new_block->nextBlock = block->nextBlock;
        //LEGE DEN NEUEN BLOCK ALS NACHFOLGER VOM ALTEN BLOCK FEST
        block->nextBlock = new_block;
    }
    //PASSE DIE LAENGE VOM ALTEN BLOCK AN
    block->dataLength = byteCount;
    block->state = allocated;
    return(block);
}


//my_free versucht den Speicherbereich, auf den <p> verweist, wieder freizugeben.
//Sofern <p> in unserem Speicher gefunden wird, muessen benachbarte, freie Speicherbloecke
//miteinander verschmolzen werden.
void my_free(void* p)
{

    if(!b_initialized)
        return;
    // SUCHE NACH DEM BLOCK MIT ZEIGER <p>
    memoryBlock* block = head;
    while(block != NULL && block->data != p)
        block = block->nextBlock;
    // FALLS KEINER GEFUNDEN WURDE, GEBE EINE MELDUNG AUS.
    if(block == NULL)
    {
        printf("Not a valid pointer\n");
        return;
    }
    //FREIGEBEN VON DEM ENTSPRECHENDEN SPEICHERBLOCK
    block->state = not_allocated;
    //FREIE SPEICHERBLOECKE MITEINANDER VERSCHMELZEN
    mergeFreeBlocks();
}

//Diese Funktion verschmilzt benachbarte, nicht benutzte Speicherbloecke
void mergeFreeBlocks()
{
    // BEGINNE AM ANFANG DER LISTE
    memoryBlock* block = head;
    while(block->nextBlock != NULL)
    {
        // WENN DER AKTUELLE UND DER BENACHBARTE BLOCK FREI SIND,
        // DANN VERSCHMELZE DIESE INDEM DIE DATENLAENGE UND DER NACHFOLGER
        // VOM AKTUELLEN BLOCK ANGEPASST WERDEN.
        if(block->state == not_allocated && block->nextBlock->state == not_allocated)
        {
            block->dataLength = block->dataLength + block->nextBlock->dataLength;
            block->nextBlock = block->nextBlock->nextBlock;
        }
        block = block->nextBlock;
    }
}

//Diese Funktion gibt eine Uebersicht ueber die vorhandenen Speicherblöcke aus
void status()
{
    if(!b_initialized)
    {
        initialize();
        b_initialized = 1;
    }
    memoryBlock* block = head;
    int count = 0;
    // printf("%d\n", memorySize);
    // printf("%d\n\n", memoryBlockHeaderSize);

    printf("\tUebersicht des Speichers: %d / %d Speicher frei\n", get_free_space(), memorySize);
    printf("\t-----------------------------------------------------------------------------------\n");
    printf("\t#  at\t\t allocated\t space\t data\t\t\t\tnext block\n");
    while(block != NULL)
    {
        printf("\t%d  %p\t %s \t\t %d\t [%11p,%11p]\t%p\n", ++count, block, boolStr[block->state], block->dataLength, (block->data), ((char*)block->data + block->dataLength-1), (block->nextBlock));
        block = block->nextBlock;
    }
    printf("\n\n");
}

