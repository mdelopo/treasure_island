#include "maputils.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <curses.h>


void print_location(WINDOW* console_window, location* l)
{
    wprintw(console_window, "\n");
    switch (l->biome) {
        case 'F':
            wprintw(console_window, " You are in the forest.\n");
            break;
        case 'B':
            wprintw(console_window, " You are on the beach.\n");
            break;
        case 'M':
            wprintw(console_window, " You are on the mountain.\n");
            break;
        case 'S':
            wprintw(console_window, " You tried to swim in the sea.\n");
            break;
    }
    wprintw(console_window, "\n");
    wrefresh(console_window);
}

const char* getfield(char* line, int num)
{
    const char* tok;
    for (tok = strtok(line, ";"); tok && *tok; tok = strtok(NULL, ";\n"))
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
        if(j++ != y) continue;
        const char* next;
        int i = 0;
        while(1)
        {
            if(i++ != x) continue;
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
    fclose(stream);
}

location** allocate_memory_for_map(int y, int x)
{
    location **map_cell = malloc(y * sizeof(location*));

    for (int i = 0; i < y; i++) {
        map_cell[i] = malloc(x * sizeof(location));
    }
    return map_cell;
}

void free_memory_for_map(location **map, int y, int x)
{
    for (int i = 0; i < y; i++) {
        free(map[i]);
    }
    free(map);
}

location** load_map_from_file(const char* filename)
{
    FILE* stream = fopen(filename, "r");
    char line[1024];
    map_file_rows = 0;
    map_file_columns = 0;
    while (fgets(line, 1024, stream)){
        map_file_rows++;
        const char* next;
        int i = 0;
        while(1)
        {
            i++;
            char* tmp = strdup(line);
            char* token = strtok(tmp, ";");
            while (token != NULL) {
                token = strtok(NULL, ";");
                if(token != NULL) i++;
            }
            free(tmp);
            break;
        }
        if (i > map_file_columns){
            map_file_columns = i;
        }
    }
    fclose(stream);

    location **map = allocate_memory_for_map(map_file_rows, map_file_columns);

    for (int i = 0; i < map_file_rows; i++) {
        for (int j = 0; j < map_file_columns; j++) {
            read_location_from_file(filename, i, j, &map[i][j]);
        }
    }
    return map;
}