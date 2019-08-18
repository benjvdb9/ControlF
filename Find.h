#ifndef HEADER_FILE
#define HEADER_FILE
#define MAX_FILE_CHARS 100

    #include <stdio.h>
    #include <stdlib.h>
    #include <fcntl.h>
    #include <unistd.h>
    #include <sys/stat.h>
    #include <sys/types.h>

    void fatal_error(void)
    {
        write(2, "ERROR", 36);
    };

    //All the info available for a thead
    struct Interval
    {
        int threadid;
        int start;
        int end;
        char *textarray;
        char *query;
    };

    //Basic text info given to main loop
    struct TextObj
    {
        int size;
        int lines;
        char *textarray;
    };

    //Analizes a text and returns basic info struct
    struct TextObj readFile(char filename[MAX_FILE_CHARS])
    {
        int size = 0;
        int lines = 1;
        char *textarray = NULL;

        int fp = open(filename, O_RDONLY);
        if (fp == -1)
        {
            write(2, "Could not open file!", 20);
        }
        else
        {
            struct stat textstats;
            stat(filename, &textstats);
            size = textstats.st_size;

            textarray = malloc(size);
            read(fp, textarray, size);

            for (int i=0; i<size; i++)
            {
                if (textarray[i] == '\n') lines++;
            }
            
            struct TextObj filetext;
            filetext.size = size + 1;
            filetext.lines = lines;
            filetext.textarray = textarray;
            close(fp);
            return filetext;
        }
    }

    char* readInput()
    {
        char c;
        int size;
        char *input = malloc(10);
        for (int i= 0; c != '\n'; i++)
        {
            read(0, &c, 1);
            size = strlen(input);
            if (size % 10 == 0)
            {
                input = realloc(input, size+10);
            }
            input[i] = c;
        }
        return input;
    } 

#endif /* HEADER_FILE */