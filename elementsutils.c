#include "elementsutils.h"
#include "maputils.h"

#include <curses.h>
#include <stdlib.h>

#include <string.h>
#include <stdio.h>


void print_element(element* element) /*Βοηθητική συνάρτηση για να τυπώνει το element struct του οποίου ο pointer της δίνεται.*/
{
    printf("\"element\" struct:\n");
    printf("Name: %s\n", element->name);
    printf("Alias: %c\n", element->alias);
    printf("Function: %c\n", element->function);
    printf("Function Amount: %f\n", element->function_amount);
    printf("inventory_effect: %s\n", element->inventory_effect);
    printf("inventory_effect_amount: %f\n", element->inventory_effect_amount);
    printf("Text: %s\n", element->text);
}

void read_elements_from_file(const char* filename, element* elements) /*Η συνάρτηση αυτή διαβάζει κάθε σειρά τους αρχείου που της δίνεται ως όρισμα
 * και χρησιμοποιώντας την sscanf αναθέτει τις ανάλογες τιμές στα στοιχεία του element-struct που αντιστοιχεί στην σειρά.
 * Παίρνει ως όρισμα το filename και ολόκληρο το array από element structs, ώστε να έχει πρόσβαση σε όλα τα elements, οπότε να χρειάζεται να καλείται μόνο μια φορά.*/
{
    FILE* stream = fopen(filename, "r");
    char line[1024];
    int i = 0;
    while (fgets(line, 1024, stream))
    {
        sscanf(line, "%[0-9a-zA-Z.,!?:| ];%c;%c;%f;%[0-9a-zA-Z.,!?:|_ ];%f;%[0-9a-zA-Z.,!?:|_ ]", elements[i].name, &elements[i].alias, &elements[i].function, &elements[i].function_amount, elements[i].inventory_effect, &elements[i].inventory_effect_amount, elements[i].text);
        i++;
    }
    fclose(stream);
}

element* allocate_memory_for_elements(int y) /*Η συνάρτηση διαθέτει (δυναμικά) χώρο στη μνήμη για τα structs που περιγράφουν το κάθε element του αρχείου.
* Η συνάρτηση παίρνει ως όρισμα το πλήθος των elements, δηλαδή τον counter y, ώστε να διαθέσει την ανάλογη μνήμη.*/
{
    element *elements = malloc(y * sizeof(element));
    return elements;
}

void free_memory_for_elements(element* elements, int y)
{
    free(elements);
}

element* load_elements_from_file(const char* filename){ /*Η συνάρτηση καλείται για να φορτωθούν στην μνήμη ως ένα array από element-structs τα elements που περιγράφονται
 * στο αρχείο που της δίνεται ως όρισμα*/
    FILE* stream = fopen(filename, "r");
    char line[1024];
    int y =0;
    while (fgets(line, 1024, stream)){ /*Εδώ περνάω σειρά-σειρά το αρχείο, αυξάνοντας τον counter y, για να διαπιστώσω πόσες σειρές κειμένου περιέχει, δηλαδή
 * πόσα elements περιγράφονται σε αυτό.*/
        y++;
    }
    fclose(stream);

    element *elements = allocate_memory_for_elements(y); /*Εδώ καλώ τη συνάρτηση, ώστε να διαθέσω (δυναμικά) χώρο στη μνήμη για τα structs που περιγράφουν το κάθε element του αρχείου.
 * Η συνάρτηση παίρνει ως όρισμα το πλήθος των elements, δηλαδή τον counter y, ώστε να διαθέσει την ανάλογη μνήμη.*/

    read_elements_from_file(filename, elements); /**/

    return elements;
}