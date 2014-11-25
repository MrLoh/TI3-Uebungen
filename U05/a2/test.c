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
    void* data;
    int dataLength;
    enum memBlockState state;
    struct _memoryblock* nextBlock;
} memoryBlock;

int main(int argc, char const *argv[])
{
    printf("%lu\n", sizeof(memoryBlock));
    return 0;
}
