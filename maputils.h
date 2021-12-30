#ifndef TREASURE_ISLAND_MAPUTILS_H
#define TREASURE_ISLAND_MAPUTILS_H

typedef struct {
    char biome;
    char elements[100];
    int num_elements;
} location;

extern int file_rows, file_columns;

void print_location(location* l);
const char* getfield(char* line, int num);
void read_location_from_file(const char* filename, int y, int x, location* p_loc);
location** allocate_memory_for_map(int y, int x);
void free_memory_for_map(location **map, int y, int x);
location** load_map_from_file(const char* filename);
#endif