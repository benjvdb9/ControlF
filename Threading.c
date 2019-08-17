#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <sched.h>
#include <string.h>
#include <sys/wait.h>
#include "Find.h"

#define STACK 1024*64

void createThread(void *message);
int find(void *interval);

int runningthreads;

int matches;
int *matchlist;

int main()
{
    matchlist = malloc(10 * sizeof(int));
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
            runningthreads = threads;
            printf("Continuing with %d threads\n", threads);
        }
    }

    struct TextObj text = readFile("text.txt");

    char *query = malloc(100);
    printf("\nFind: ");

    fgets(query, 100, stdin);

    if (query[strlen(query) - 1] == '\n')
        query[strlen(query) - 1] = '\0';

    int length = text.size / threads - 1;
    int rest     = text.size % threads;

    int startpos;
    int endpos = -1;
    for (int i=0; i<threads; i++)
    {
        struct Interval interval;
        interval.threadid = 0;

        startpos = endpos + 1;
        endpos = startpos + length;
        if (rest > 0)
        {
            endpos++;
            rest--;
        }

        struct Interval *malloc_container = 
        (struct Interval *) malloc(sizeof(struct Interval));

        interval.start = startpos;
        interval.end   = endpos;
        interval.textarray = text.textarray;
        interval.query = query;

        malloc_container = &interval;
        createThread(malloc_container);
    }

    int pos = 0;
    int line = 1;
    int column = 1;
    for (int i=0; i<=text.size; i++)
    {
        if (text.textarray[i] == '\n')
        {
            line++;
            column =0;
        }
        for (int j=0; j<matches; j++)
        {
            if (matchlist[j] == pos)
            {
                printf("MATCH FOUND AT %d: LINE %d, COLUMN %d\n"
                , pos, line, column);
            }
        }
        pos++;
        column++;
    }
    return 0;
}

void createThread(void *message)
{
    void *stack;
    pid_t pid;

    stack = malloc(STACK);
    if (stack == 0) {
        perror("No stack allocation");
        exit(1);
    }

    pid = clone(*find, stack + STACK, SIGCHLD | CLONE_VM, message);

    if (pid == -1)
    {
        perror("clone");
        exit(2);
    }

    wait(NULL);
    free(stack);
}

int find(void *container)
{
    struct Interval *interval;
    interval = (struct Interval *) container;

    int id = interval->threadid;
    int start = interval->start;
    int end   = interval->end;
    char *characters = interval->textarray;
    char *query = interval->query;

    char c;
    int position;
    int line = 1;
    int queryindex = 0;
    for(int pos =start; pos<=end; pos++)
    {
        c = characters[pos];
        if (c == query[queryindex])
        {
            if(queryindex == strlen(query) - 1)
            {
                if (queryindex == 0) position = pos;
                else queryindex = 0;

                if (matches % 10 == 0)
                {
                    matchlist = realloc(matchlist, matches+10);
                }
                matchlist[matches] = position;
                matches++;
            }
            else if (queryindex == 0)
            {
                position = pos;
                queryindex++;
            }
            else
            {
                queryindex++;
            }
            if (pos == end) end++;
        }
        else
        {
            queryindex = 0;
        }
    }

    runningthreads--;
    if (!runningthreads)
    {
        free(query);
        free(characters);
    }
    return 0;
}