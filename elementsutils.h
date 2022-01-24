#ifndef TREASURE_ISLAND_ELEMENTSUTILS_H
#define TREASURE_ISLAND_ELEMENTSUTILS_H

#include <curses.h>

/*Η συγκεκριμένη custom βιβλιοθήκη συνοδεύεται από το elementsutils.c και γράφτηκε για να οργανώσει της συναρτήσεις που ασχολούνται με την δημιουργία των element-structs και
 * την φόρτωσή τους στη μνήμη από το elements-config αρχείο.*/

typedef struct { /* Εδώ ορίζω τον τύπο ενός struct το οποίο ονομάζω "element". Θα δημιουργώ structs τέτοιου τύπου ώστε να αποθηκεύω με συνεπή τρόπο τις ιδιότητες των διαφόρων αντικειμένων
 * που θα βρίσκει ο παίκτης καθώς κινείται στον χάρτη. Τα αντικείμενα αυτά περιγράφονται στο elements_file, το οποίο μπορεί να τροποποιηθεί, ώστε να αλλάξει ο τρόπος που παίζεται το παιχνίδι,
 * εφόσον ακολουθείται η δομή μου περιγράφω στο έγγραφο τεκμηρίωσης.*/
    char name[51];
    char alias;
    char function;
    float function_amount;
    char inventory_effect[100];
    float inventory_effect_amount;
    char text[300];
} element;

void print_element(element* e);

void read_elements_from_file(const char* filename, element* elements);
element* allocate_memory_for_elements(int y);
void free_memory_for_elements(element* elements, int y);
element* load_elements_from_file(const char* filename);


#endif
