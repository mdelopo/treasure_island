#ifndef TREASURE_ISLAND_GAMEUTILS_H
#define TREASURE_ISLAND_GAMEUTILS_H
#include <curses.h>
#include "elementsutils.h"
#include "maputils.h"


typedef struct {
    char current_biome;
    float health;
    int current_x;
    int next_x;
    int current_y;
    int next_y;
    bool game_over;
    bool has_treasure;
    int shark_counter;
} _player;

void initialize_player(_player* player);
void action(WINDOW* console_window, location **map, _player* p_player, element* elements);

void damage(WINDOW* console_window, _player* p_player, element* p_element);
void heal(WINDOW* console_window, _player* p_player, element* p_element);

void death(WINDOW* console_window, _player* p_player);
void win(WINDOW* console_window, _player* p_player);

#endif //TREASURE_ISLAND_GAMEUTILS_H
