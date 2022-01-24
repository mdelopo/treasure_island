#ifndef TREASURE_ISLAND_TERMINALUTILS_H
#define TREASURE_ISLAND_TERMINALUTILS_H
#include <curses.h>
#include "maputils.h"
#include "gameutils.h"

/*Η συγκεκριμένη custom βιβλιοθήκη συνοδεύεται από το terminalutils.c και γράφτηκε για να οργανώσει τις συναρτήσεις που ασχολούνται με το τερματικό, δηλαδή την εκτύπωση του χάρτη,
 * την εκτύπωση της κατάστασης του παίκτη (health και inventory), καθώς και το resizing του τερματικού.*/

void resize_window(WINDOW* console_box, WINDOW* console_window, WINDOW* map_box, WINDOW* map_window, WINDOW* status_box, WINDOW* status_window, location **map);
void print_map_point(WINDOW* map_window, int i, int j, int color_pair_num);
void print_map(WINDOW* map_window, location **map);

void print_player_status(WINDOW* status_window, _player* p_player);
#endif //TREASURE_ISLAND_TERMINALUTILS_H
