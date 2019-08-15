#ifndef HEADER_FILE
#define HEADER_FILE
#define MAX_FILE_CHARS 100

    #include <stdio.h>

    void readFile(char filename[MAX_FILE_CHARS]){
        int i, j;
        char c;
        char *textline[500];

        FILE *fp = fopen(filename, "r");
        if (fp == NULL){
            printf("Could not open file");
        } else {
            for(c = getc(fp); c != EOF; c = getc(fp))
            {
                if(c == '\n'){
                    j = 0;
                    memset(textline, 0, 500);
                    i = i+1;
                } else {
                    textline[j]= c;
                    j=j+1; 
                }
            }
        }
    }

#endif /* HEADER_FILE */