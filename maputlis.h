#ifndef MAPUTILS_H
#define MAPUTILS_H

typedef struct {
    char biome;
    char elements[100];
    int num_elements;
} location;

void print_location(location* l);
const char* getfield(char* line, int num);
void read_location_from_file(const char* filename, int y, int x, location* p_loc);


#endif