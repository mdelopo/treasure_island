#ifndef TREASURE_ISLAND_GAMEUTILS_H
#define TREASURE_ISLAND_GAMEUTILS_H
#include <curses.h>
#include "elementsutils.h"
#include "maputils.h"

#define INVENTORY_SIZE 3
/*Η συγκεκριμένη custom βιβλιοθήκη συνοδεύεται από το gameutils.c και γράφτηκε για να οργανώσει τις συναρτήσεις που ασχολούνται με ο gameplay.*/
typedef struct {/* Εδώ ορίζω τον τύπο ενός struct το οποίο ονομάζω "element" και περιλαμβάνει όλες τις μεταβλητές για να αποθηκεύει την κατάσταση του παίκτη.
 * Έτσι όλες οι συναρτήσεις που χρειάζονται δεδομένα του παίκτη μπορεί να παίρνει τον pointer ενός τέτοιου struct που δημιουργώ στην αρχή του προγράμματος και
 * να έχει πρόσβαση σε όλα τα στοιχεία, αντί να χρειάζεται να περνάνε όλα ξεχωριστά.*/
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

char* user_select_map_filename();
char* user_select_elements_filename();

void initialize_player(_player* player, location** map);
int probability_1_100(int p);

int shark_encounter(WINDOW* console_window, _player* p_player);

float compass_navigation_accuracy_increase(_player* p_player);
void movement(_player* p_player, int direction);
void action(WINDOW* console_window, location **map, _player* p_player, element* elements);

void inventory(WINDOW* console_window, location **map, _player* p_player, element* p_element);

float damage_reduction(_player* p_player);
void damage(WINDOW* console_window, _player* p_player, element* p_element);

void heal(WINDOW* console_window, _player* p_player, element* p_element);

void death(WINDOW* console_window, _player* p_player);
void win(WINDOW* console_window, _player* p_player);

#endif //TREASURE_ISLAND_GAMEUTILS_H
