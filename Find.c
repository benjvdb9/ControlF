#include <stdio.h>
#include "Find.h"

int main()
{
    FILE  * file = fopen("text.txt", "r");

    fclose(file);
    return 0;
}