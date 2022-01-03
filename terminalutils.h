#ifndef TREASURE_ISLAND_TERMINALUTILS_H
#define TREASURE_ISLAND_TERMINALUTILS_H
#include <curses.h>
#include "maputils.h"

void resize_window(WINDOW* console_box, WINDOW* console_window, WINDOW* map_box, WINDOW* map_window, WINDOW* status_box, WINDOW* status_window);
void print_map_point(WINDOW* map_window, int i, int j, int color_pair_num);
void print_map(WINDOW* map_window, location **map);
#endif //TREASURE_ISLAND_TERMINALUTILS_H
