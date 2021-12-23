#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char* getfield(char* line, int num)
{
    const char* tok;
    for (tok = strtok(line, ";");
            tok && *tok;
            tok = strtok(NULL, ";\n"))
    {
        if (!--num) return tok;
    }
    return NULL;
}

int main()
{
    FILE* stream = fopen("island_default.csv", "r");

    char line[1024];
    while (fgets(line, 1024, stream))
    {
        const char* next = (char*)1;
        int i = 0;
        while(1)
        {
            i++;
            char* tmp = strdup(line);
            next = getfield(tmp, i); // NOTE strtok clobbers tmp
            if(next == NULL) break;
            printf("%4s ", next);
            free(tmp);
        }
        printf("\n");  
    }
}