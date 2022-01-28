#include "maputils.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <curses.h>


void print_location(WINDOW* console_window, location* l) /*Συνάρτηση που καλείται κάθε φορά που ο παίκτης αλλάζει θέση, ώστε να τον ενημερώνει για τον τύπο του εδάφους.*/
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

const char* getfield(char* line, int num) /*Συνάρτηση από stackoverflow, η οποία δέχεται μία γραμμή και έναν αριθμός και επιστρέφει το κελί της γραμμής που βρίσκεται στη στήλη του αριθμού.*/
{
    const char* tok;
    for (tok = strtok(line, ";"); tok && *tok; tok = strtok(NULL, ";\n"))
    {
        if (!--num) return tok;
    }
    return NULL;
}

void read_location_from_file(const char* filename, int y, int x, location* p_loc)/*Η συνάρτηση παίρνει ως όρισμα το όνομα του αρχείου χάρτη, τη στήλη και τη σειρά και τον pointer που
 * αντιστοιχεί στο κελί που ζητείται να φορτωθεί στη μνήμη. Ανοίγει το αρχείο και περνάει σειρά-σειρά και στήλη-στήλη, ώσπου να βρει το ζητούμενο κελί και αποθηκεύει τις πληροφορίες του
 * στη μνήμη στο location-struct.*/
{
    FILE* stream = fopen(filename, "r");
    char line[1024];
    int j = 0;
    while (fgets(line, 1024, stream)) /*Εδώ διαβάζει σειρά-σειρά το αρχείο και μεταβιβάζει τις σειρές στη while για να διαβαστούν κελί-κελί.*/
    {
        if(j++ != y) continue; /*Εδώ τσεκάρει αν η σειρά που είναι αποθηκευμένη στο line είναι η ζητούμενη. Αν δεν είναι συνεχίζει στην επόμενη επανάληψη.*/
        const char* next;
        int i = 0;
        while(1)
        {
            if(i++ != x) continue;
            char* tmp = strdup(line);
            next = getfield(tmp, i); /*Εδώ καλείται η πάνω συνάρτηση και αν ο pointer που επιστρέφει έχει περιεχόμενο το αποθηκεύω στο location-struct.
 * Αλλιώς γίνεται break και το πρόγραμμα συνεχίζει στην επόμενη σειρά.*/
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

location** allocate_memory_for_map(int y, int x)/*Η συνάρτηση διαθέτει (δυναμικά) χώρο στη μνήμη για τα structs που περιγράφουν το κάθε σημείο του χάρτη.
* Η συνάρτηση παίρνει ως όρισμα το πλήθος τις διαστάσεις x και y, ώστε να διαθέσει την ανάλογη μνήμη.*/
{
    location **map_cell = malloc(y * sizeof(location*));

    for (int i = 0; i < y; i++) {
        map_cell[i] = malloc(x * sizeof(location));
    }
    return map_cell;
}

void free_memory_for_map(location **map, int y, int x){/*Βοηθητική συνάρτηση που απελευθερώνει τη μνήμη που έχει δεσμευτεί για την αποθήκευση του χάρτη.*/
    for (int i = 0; i < y; i++) {
        free(map[i]);
    }
    free(map);
}

location** load_map_from_file(const char* filename)/*Η συνάρτηση καλείται για να φορτωθούν στη μνήμη ως ένα array από arrays (ώστε να προκύπτει δισδιάστατος πίνακας που περιλαμβάνει
 * location stucts στα κελιά) από location-structs τα σημεία του χάρτη που περιγράφονται στο αρχείο που της δίνεται ως όρισμα*/
{
    FILE* stream = fopen(filename, "r");
    char line[1024];
    map_file_rows = 0;
    map_file_columns = 0;
    while (fgets(line, 1024, stream)){//Εδώ διαβάζω σειρά-σειρά το αρχείο, ώστε να διαπιστώσω πόσες γραμμές έχει ο χάρτης.
        map_file_rows++;
        const char* next;
        int i = 0;
        while(1)
        {
            i++;
            char* tmp = strdup(line);
            char* token = strtok(tmp, ";");
            while (token != NULL) {
                token = strtok(NULL, ";"); /*εδώ περνάω κάθε φορά το κείμενο της σειράς μέχρι να συναντήσω το επόμενο ερωτηματικό ΄;',
 * ώστε να σπάσω τη σειρά σε κελιά και έτσι να βρώ πόσες στήλες υπάρχουν. Για κάθε στήλη αυξάνω τον counter i κατά ένα.*/
                if(token != NULL) i++;
            }
            free(tmp);
            break;
        }
        if (i > map_file_columns){/*Εδώ τσεκάρω αν το i είναι μεγαλύτερο από το μέγιστο πλήθος τον στηλών που έχω ήδη εντοπίσει στις προηγούμενες σειρές.
 * Έτσι μπορεί μία σειρά να έχει λιγότερες στήλες, αλλά και πάλι θα δεσμευτεί αρκετή μνήμη.*/
            map_file_columns = i;
        }
    }
    fclose(stream);

    location **map = allocate_memory_for_map(map_file_rows, map_file_columns);/*Εδώ καλώ τη συνάρτηση, ώστε να διαθέσω (δυναμικά) χώρο στη μνήμη για τα structs που περιγράφουν το κάθε element του αρχείου.
 * Η συνάρτηση παίρνει ως όρισμα το πλήθος των elements, δηλαδή τον counter y, ώστε να διαθέσει την ανάλογη μνήμη.*/

    for (int i = 0; i < map_file_rows; i++) {
        for (int j = 0; j < map_file_columns; j++) {
            read_location_from_file(filename, i, j, &map[i][j]); /*Εδώ καλώ τη συνάρτηση που φορτώνει στη μνήμη το σημείο του χάρτη από το αρχείο, του οποίου οι συντεταγμένες i,j της δίνονται.*/
        }
    }
    return map;
}