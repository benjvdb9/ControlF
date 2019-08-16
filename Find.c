#include <stdio.h>
#include "Find.h"

int main()
{
    struct TextObj text = readFile("text.txt");
    printf("\nSize: %d\n", text.size);
    printf("\nLines: %d\n\n", text.lines);

    for(int i = 0; i<4110; i++)
    {
        printf("%c", text.textarray[i]);
    }
}