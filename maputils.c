#include "maputlis.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>


void print_location(location* l)
{
    printf("\"location\" struct:\n");
    printf("Address: %p\n", l);
    printf("Biome: %c\n", l->biome);
    printf("Elements (%i): ", l->num_elements);
    for(int i=0; i<l->num_elements; i++) printf("%c ", l->elements[i]);
    printf("\n");
}

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

void read_location_from_file(const char* filename, int y, int x, location* p_loc)
{
    FILE* stream = fopen(filename, "r");
    char line[1024];
    int j = 0;
    while (fgets(line, 1024, stream))
    {
        if(++j!=y) continue;
        const char* next;
        int i = 0;
        while(1)
        {
            i++;
            if(i!=x) continue;
            char* tmp = strdup(line);
            next = getfield(tmp, i); // NOTE strtok clobbers tmp
            if(next == NULL) break;
            p_loc->biome = next[0];
            p_loc->num_elements = strlen(next)-1;
            strcpy(p_loc->elements, next+1);
            free(tmp);
            break;
        }
    }
}