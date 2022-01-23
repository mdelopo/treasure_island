#ifndef TREASURE_ISLAND_GAMEUTILS_H
#define TREASURE_ISLAND_GAMEUTILS_H
#include <curses.h>
#include "elementsutils.h"
#include "maputils.h"

#define INVENTORY_SIZE 3

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
    element* inventory[INVENTORY_SIZE];
} _player;

void initialize_player(_player* player);
int probability_1_100(int p);

int shark_encounter(WINDOW* console_window, _player* p_player);
void movement(WINDOW* console_window,WINDOW* map_window, location **map, _player* p_player, element* elements);

void action(WINDOW* console_window, location **map, _player* p_player, element* elements);

void inventory(WINDOW* console_window, location **map, _player* p_player, element* p_element);

float damage_reduction(_player* p_player);
void damage(WINDOW* console_window, _player* p_player, element* p_element);

void heal(WINDOW* console_window, _player* p_player, element* p_element);

void death(WINDOW* console_window, _player* p_player);
void win(WINDOW* console_window, _player* p_player);

#endif //TREASURE_ISLAND_GAMEUTILS_H
