#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <sched.h>
#include <sys/wait.h>
#include "Find.h"

#define STACK 1024*64

void createThread(void *message);
int find(void *interval);

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
    int length = text.size / threads - 1;
    int rest     = text.size % threads;

    int startpos;
    int endpos = -1;
    for (int i=0; i<threads; i++)
    {
        startpos = endpos + 1;
        endpos = startpos + length;
        if (rest > 0)
        {
            endpos++;
            rest--;
        }

        struct Interval *malloc_container = 
        (struct Interval *) malloc(sizeof(struct Interval));

        struct Interval interval;
        interval.start = startpos;
        interval.end   = endpos;

        malloc_container = &interval;
        createThread(malloc_container);
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

    int start = interval->start;
    int end   = interval->end;

    printf("%d\n", start);
    printf("%d\n", end);
    return 0;
}