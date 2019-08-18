#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

int main()
{
    int filedesc = open("test.txt", O_RDONLY);

    struct stat v;
    stat("test.txt", &v);
    int size = v.st_size;
    char *data = malloc(size);

    int t = read(filedesc, data, size);
    printf("%s\n", data);
}