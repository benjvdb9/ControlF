#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>

int main()
{
    /*int filedesc = open("test.txt", O_RDONLY);

    struct stat v;
    stat("test.txt", &v);
    int size = v.st_size;
    char *data = malloc(size);

    int t = read(filedesc, data, size);
    printf("%s\n", data);*/

    char c;
    int size;
    char *test = malloc(10);
    read(0, &c, 1);
    for (int i= 0; c != '\n'; i++)
    {
        size = strlen(test);
        if (size % 10 == 0)
        {
            test = realloc(test, size+10);
        }
        test[i] = c;
        read(0, &c, 1);
    }
    printf("%s, %ld\n", test, strlen(test));
}