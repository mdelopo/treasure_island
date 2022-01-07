#include "elementsutils.h"
#include "maputils.h"

#include <curses.h>
#include <stdlib.h>

#include <string.h>
#include <stdio.h>


void print_element(element* element)
{
    printf("\"element\" struct:\n");
    printf("Alias: %c\n", element->alias);
    printf("Function: %c\n", element->function);
    printf("Function Amount: %f\n", element->function_amount);
    printf("Text: %s\n", element->text);
}

void read_elements_from_file(const char* filename, element* elements)
{
    FILE* stream = fopen(filename, "r");
    char line[1024];
    int i = 0;
    while (fgets(line, 1024, stream))
    {
        sscanf(line, "%c;%c;%f;%[0-9a-zA-Z.,!?:| ]s", &elements[i].alias, &elements[i].function, &elements[i].function_amount, elements[i].text);
        i++;
    }
    fclose(stream);
}

element* allocate_memory_for_elements(int y)
{
    element *elements = malloc(y * sizeof(element));
    return elements;
}

void free_memory_for_elements(element* elements, int y)
{
    free(elements);
}

element* load_elements_from_file(const char* filename){
    FILE* stream = fopen(filename, "r");
    char line[1024];
    int y =0;
    while (fgets(line, 1024, stream)){
        y++;
    }
    fclose(stream);

    element *elements = allocate_memory_for_elements(y);

    read_elements_from_file(filename, elements);

    return elements;
}