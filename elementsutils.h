#ifndef TREASURE_ISLAND_ELEMENTSUTILS_H
#define TREASURE_ISLAND_ELEMENTSUTILS_H

#include <curses.h>

typedef struct {
    char alias;
    char function;
    float function_amount;
    char text[300];
} element;

void print_element(element* e);

void read_elements_from_file(const char* filename, element* elements);
element* allocate_memory_for_elements(int y);
void free_memory_for_elements(element* elements, int y);
element* load_elements_from_file(const char* filename);


#endif
