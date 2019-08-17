#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include "Find.h"

int main()
{
    int threads = 0;
    char term;
    char scanfbufferbin[100];
    while (threads < 1)
    {
        printf("How many threads? : ");
        int scan = scanf("%d%c", &threads, &term);
        if (scan == 0)
        {
            scanf("%s", scanfbufferbin);
            printf("Please input a number\n");
            threads = 0;
        }
        else if(term != '\n')
        {
            printf("Non-digit character detected\n");
            threads = 0;
        }
        else if (threads < 1)
        {
            printf("Need atleast 1 thread!\n");
            threads = 0;
        }
        else
        {
            printf("Continuing with %d threads\n", threads);
        }
    }

    struct TextObj text = readFile("text.txt");
    printf("%d characters, %d threads\n", text.size, threads);
    return 0;
}