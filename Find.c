#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Find.h"

int main()
{
    /***
     * NOTE:
     *         PRINTING INFLUENCES RESULTS SOMEHOW.
     *         KEEP PRINTS AT A MINIMUM !!!
     * ***/

    //DEMO CODE / NO THREADING
    struct TextObj text = readFile("text.txt");
    printf("Size: %d", text.size);
    printf("\nLines: %d\n\n", text.lines);

    //Print file text
    for(int i = 0; i<=text.size; i++)
    {
        printf("%c", text.textarray[i]);
    }

    char *query = malloc(100);
    printf("\nFind: ");

    //Read input + overflow protection
    fgets(query, 100, stdin);

    //Erase trailing newline
    if (query[strlen(query) - 1] == '\n')
        query[strlen(query) - 1] = '\0';

    //Char content analysis
    for(int i=0; i<strlen(query); i++)
    {
        printf("Char #%d: %c\n", i, query[i]);
    }
    printf("\n");

    //Main code loop to thread
    char c;
    int position;
    int line = 1;
    int queryindex = 0;
    fflush(stdout);
    for(int i =0; i<=text.size; i++)
    {
        c = text.textarray[i];
        if (c == '\n') line++;

        if (c == query[queryindex])
        {
            if(queryindex == strlen(query) - 1)
            {
                if (queryindex == 0) position = i;
                else queryindex = 0;
                printf("\nMATCH FOUND AT %d LINE %d", position, line);
                fflush(stdout);
            }
            else if (queryindex == 0)
            {
                position = i;
                queryindex++;
            }
            else
            {
                queryindex++;
            }
        }
        else
        {
            queryindex = 0;
        }
    }
    printf("\nEND SEARCH\n");

    free(query);
    free(text.textarray);
    return 0;
}