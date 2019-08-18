#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <sched.h>
#include <string.h>
#include <sys/wait.h>
#include <pthread.h>
#include "Find.h"

#define STACK 1024*64

void createThread(void *message);
int find(void *interval);

int runningthreads;

int matches;
int *matchlist;
pthread_mutex_t lock;

int main()
{
    //User inputs parsing
    matchlist = malloc(10 * sizeof(int));
    int threads = 0;
    char term;
    char scanfbufferbin[100];
    while (threads < 1)
    {
        write(1, "How many threads? : ", 20);
        int scan = scanf("%d%c", &threads, &term);
        if (scan == 0)
        {
            scanf("%s", scanfbufferbin);
            write(1, "Please input a number\n", 22);
            threads = 0;
        }
        else if(term != '\n')
        {
            write(1, "Non-digit character detected\n", 29);
            threads = 0;
        }
        else if (threads < 1)
        {
            write(1, "Need atleast 1 thread!\n", 23);
            threads = 0;
        }
        else
        {
            runningthreads = threads;
            printf("Continuing with %d threads\n", threads);
        }
    }

    char *filename;
    write(1, "\nFile to search: ", 17);
    filename = readInput();
    filename[strlen(filename) - 1] = '\0';
    struct TextObj text = readFile(filename);
    free(filename);

    char *outputfile;
    write(1, "\nOutput file: ", 14);
    outputfile = readInput();
    outputfile[strlen(outputfile) - 1] = '\0';

    char *query = malloc(100);
    write(1, "\nFind: ", 7);
    fgets(query, 100, stdin);
    //end parsing

    if (query[strlen(query) - 1] == '\n')
        query[strlen(query) - 1] = '\0';

    int length = text.size / threads - 1;
    int rest     = text.size % threads;

    int startpos;
    int endpos = -1;
    pthread_mutex_init(&lock, NULL);

    //Setup and creation of the threads
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

    //Post-thread roundup: get coordinates and write output file
    int pos = 0;
    int line = 1;
    int column = 0;
    int of = open(outputfile, O_WRONLY | O_TRUNC | O_CREAT, 0777);
    if (of < 0) perror("Couldn't open file");
    for (int i=0; i<=text.size; i++)
    {
        column++;
        if (text.textarray[i] == '\n')
        {
            line++;
            column =0;
        }
        for (int j=0; j<matches; j++)
        {
            if (matchlist[j] == pos)
            {
                char str[40];
                sprintf(str, "MATCH FOUND AT %d: LINE %d, COLUMN %d\n"
                , pos, line, column);
                printf("%s", str);
                int res = write(of, str, 40);
                if (res < 0) perror("Couldn't write in file");
            }
        }
        pos++;
    }
    char conc[30];
    sprintf(conc, "\n%d MATCHES FOUND IN TOTAL.\n", matches);
    printf("%s", conc);
    int res2 = write(of, conc, 30);
    if (res2 < 0) perror("Couldn't write in file");
    close(of);

    free(outputfile);
    free(matchlist);
    free(text.textarray);
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

/*Within an interval, compare every character to the query's characters
if match and reaching interval end allow for overtime as long as characters
still match*/
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

                pthread_mutex_lock(&lock);
                if (matches % 10 == 0)
                {
                    int next = matches + 10;
                    matchlist = realloc(matchlist, next*sizeof(int));
                }
                matchlist[matches] = position;
                matches++;
                pthread_mutex_unlock(&lock);
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

    //free query malloc once all threads finished
    pthread_mutex_lock(&lock);
    runningthreads--;
    pthread_mutex_unlock(&lock);
    if (!runningthreads)
    {
        free(query);
    }
    return 0;
}