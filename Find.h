#ifndef HEADER_FILE
#define HEADER_FILE
#define MAX_FILE_CHARS 100

    #include <stdio.h>
    #include <stdlib.h>

    void fatal_error(void)
    {
        printf("ERROR");
    };

    struct TextObj
    {
        int size;
        int lines;
        char *textarray;
    };

    struct TextObj readFile(char filename[MAX_FILE_CHARS])
    {
        int size = 0;
        int lines = 0;
        int buffer_size = 0;

        char c;
        char *tempbuf  = NULL;
        char *textarray = NULL;

        FILE *fp = fopen(filename, "r");
        if (fp == NULL){
            printf("Could not open file");
        } else {
            for(c = getc(fp); c != EOF; c = getc(fp))
            {
                if (size == buffer_size){
                    buffer_size += 100;
                    tempbuf = realloc(textarray, buffer_size);
                    if (!tempbuf) fatal_error();
                    textarray = tempbuf;
                }

                if (c == '\n') lines++;

                textarray[size]= c;
                ++size;
            }
            
            struct TextObj filetext;
            filetext.size = size;
            filetext.lines = lines;
            filetext.textarray = textarray;
            return filetext;
        }
    }

#endif /* HEADER_FILE */