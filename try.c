#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    FILE* fp;
    char buffer[255];

    fp = fopen("infor.txt", "r");

    while(fgets(buffer, 255, (FILE*) fp)) {
    printf("%s", buffer);
    }
    printf("\n");

    fclose(fp);
}
