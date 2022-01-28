#ifndef TREASURE_ISLAND_MAPUTILS_H
#define TREASURE_ISLAND_MAPUTILS_H
#include <curses.h>

/*Η συγκεκριμένη custom βιβλιοθήκη συνοδεύεται από το maputils.c και γράφτηκε για να οργανώσει τις συναρτήσεις που ασχολούνται με την δημιουργία των location-structs για κάθε σημείο του χάρτη,
 *και την φόρτωσή τους στη μνήμη από το map-config αρχείο.*/
typedef struct {/* Εδώ ορίζω τον τύπο ενός struct το οποίο ονομάζω "location". Θα δημιουργώ structs τέτοιου τύπου ώστε να αποθηκεύω με συνεπή τρόπο το κάθε σημείο του χάρτη, δηλαδή
 * τον τύπο του εδάφους και το element που πιθανόν να βρει ο παίκτης σε εκείνο το σημείο.
 * Ο χάρτης περιγράφονται στο map_file, το οποίο μπορεί να τροποποιηθεί, ώστε να είναι εύκολη η παραμετροποίηση του παιχνιδιού,
 * εφόσον ακολουθείται η δομή μου περιγράφω στο έγγραφο τεκμηρίωσης.*/
    char biome;
    char elements[100];
    int num_elements;
} location;

extern int map_file_rows, map_file_columns;

void print_location(WINDOW* console_window, location* l);
const char* getfield(char* line, int num);
void read_location_from_file(const char* filename, int y, int x, location* p_loc);

location** allocate_memory_for_map(int y, int x);
void free_memory_for_map(location **map, int y, int x);
location** load_map_from_file(const char* filename);
#endif