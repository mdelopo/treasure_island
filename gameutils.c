#include "gameutils.h"
#include "maputils.h"
#include "elementsutils.h"
#include <curses.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <dirent.h>


char* user_select_map_filename(){ /*Η συνάρτηση αυτή καλείται για να ρωτήσει τον παίκτη ποιο αρχείο-χάρτη να χρησιμοποιήσει το πρόγραμμα. Επιστρέφει το όνομα του αρχείου.*/
    DIR *d;
    struct dirent *dirs[100];
    d = opendir("./islands");
    printf("\nType the number of the map-file you want to load and press ENTER. To use the default map type 0.\n");
    int count = 1;
    if (d) {
        while ((dirs[count] = readdir(d)) != NULL) {
            if (strcmp(dirs[count]->d_name, "..") != 0 && strcmp(dirs[count]->d_name, ".") != 0) {
                printf("%d %s\n", count, dirs[count]->d_name);
            }
            else{
                count--;
            }
            count++;
        }
        closedir(d);
    }
    count--;
    int input_load_map = 0;
    scanf("%d", &input_load_map);
    if(input_load_map == 0){
        char* map_filename = strdup("./islands/island_default.csv");
        printf("Loading %s...\n", map_filename);
        return map_filename;
    }
    else if(input_load_map>0 && input_load_map<=count){
        char* map_folder = "islands/";
        char* map_filename = malloc(strlen(map_folder)+strlen(dirs[input_load_map]->d_name)+1);
        strcpy(map_filename, map_folder);
        strcat(map_filename, dirs[input_load_map]->d_name);
        printf("Loading %s...\n", map_filename);
        return map_filename;
    }
    else{
        printf("Item not found. Please restart and try again.\n");
        exit(0);
    }
}
char* user_select_elements_filename(){/*Η συνάρτηση αυτή καλείται για να ρωτήσει τον παίκτη ποιο αρχείο-στοιχείων να χρησιμοποιήσει το πρόγραμμα. Επιστρέφει το όνομα του αρχείου.*/
    DIR *d;
    struct dirent *dirs[100];
    d = opendir("./elements");
    printf("\nType the number of the elements-file you want to load and press ENTER. To use the default elements-config-file type 0.\n");
    int count = 1;
    if (d) {
        while ((dirs[count] = readdir(d)) != NULL) {
            if (strcmp(dirs[count]->d_name, "..") != 0 && strcmp(dirs[count]->d_name, ".") != 0) {
                printf("%d %s\n", count, dirs[count]->d_name);
            }
            else{
                count--;
            }
            count++;
        }
        closedir(d);
    }
    count--;
    int input_load_elements = 0;
    scanf("%d", &input_load_elements);
    if(input_load_elements == 0){
        char* elements_filename = strdup("./elements/elements_default.csv");
        printf("Loading %s...\n", elements_filename);
        return elements_filename;
    }
    else if(input_load_elements>0 && input_load_elements<=count){
        char* elements_folder = "elements/";
        char* elements_filename = malloc(strlen(elements_folder)+strlen(dirs[input_load_elements]->d_name)+1);
        strcpy(elements_filename, elements_folder);
        strcat(elements_filename, dirs[input_load_elements]->d_name);
        printf("Loading %s...\n", elements_filename);
        return elements_filename;
    }
    else{
        printf("Item not found. Please restart and try again.\n");
        exit(0);
    }
}

void initialize_player(_player* player, location** map){/*Με αυτή τη συνάρτηση αρχικοποιείται το player-struct*/
    player->current_x = 2;
    player->current_y = 2;
    for(int x = 0; x<map_file_columns; x++){
        for(int y = 0; y<map_file_rows; y++){
            if(map[y][x].elements[0]=='@'){ /*Εδώ βρίσκει το κελί του χάρτη που έχει το '@' ώστε να θέσει εκεί τις αρχικές συντεταγμένες του παίκτη.*/
                player->current_x = x;
                player->current_y = y;
            }
        }
    }
    player->next_x = player->current_x;
    player->next_y = player->current_y;
    player->health = 10;
    player->game_over = false;
    player->shark_counter = 0;
    for (int i = 0; i < INVENTORY_SIZE; ++i) {
        player->inventory[i] = NULL;
    }
    srand(time(0));
}
int probability_1_100(int p){ /*Η συνάρτηση παίρνει ως όρισμα μία πιθανότητα*% και έπειτα παράγει έναν τυχαίο αριθμό από το 0 εως το 100. Έπειτα συγκρίνει
 * τους δύο αριθμούς. Επιστρέφει '1' αν ο τυχαίος αριθμός είναι <= του ορίσματος και '0' αν είναι μεγαλύτερος. Έτσι μπορεί να χρησιμοποιηθεί για να βρεθεί αν συνέβη ένα γεγονός
 * στο παιχνίδι που συμβαίνει με κάποια πιθανότητα. 1-> συνέβη, 0-> δεν συνέβη*/
    int num = rand()%(100+1);
    if (num <= p) return 1;
    else return 0;
}

int shark_encounter(WINDOW* console_window,  _player* p_player){ /*Η συνάρτηση καλείται όταν ο παίκτης συναντάει τους καρχαρίες.*/
    int sharks_appetite[3] = {20, 70, 100};
    int result = probability_1_100(sharks_appetite[p_player->shark_counter - 1]);
    if(result == 1){
        p_player->health = 0;
        death(console_window, p_player);
    }
}
float compass_navigation_accuracy_increase(_player* p_player){ /*Η συνάρτηση καλείται για να βρεθεί αν ο παίκτης έχει αντικείμενα στην κατοχή του που βελτιώνουν την ακρίβεια του προσανατολισμού του.
 * Σκανάρει θέση-θέση το inventory του παίκτη και αν υπάρχουν αντικείμενα που έχουν ως inventory_effect το "navigation_accuracy_increase" αυξάνει κατά το "inventory_effect_amount" το
 * "navigation_accuracy_increase_value", το οποίο είναι ένα float που επιστρέφει η συνάρτηση.*/
    float navigation_accuracy_increase_value = 1;
    for (int i = 0; i < INVENTORY_SIZE; i++) {
        if(p_player->inventory[i]!=NULL && strcmp(p_player->inventory[i]->inventory_effect, "navigation_accuracy_increase") == 0){
            navigation_accuracy_increase_value = navigation_accuracy_increase_value + p_player->inventory[i]->inventory_effect_amount;
        }
    }
    if(navigation_accuracy_increase_value<0) navigation_accuracy_increase_value=0;
    return navigation_accuracy_increase_value;
}

void movement(_player* p_player, int direction){ /*Η συνάρτηση αυτή καλείται όταν ο παίκτης πατάει τα πλήκτρα που αναφέρονται στην κίνησή του. Χρησιμοποιεί την
 * probability_1_100 με όρισμα την πιθανότητα σωστής κίνησης επί την compass_navigation_accuracy_increase για να βρει την κατεύθυνση κίνησης του παίκτη.*/
    enum directions{UP, RIGHT, DOWN, LEFT};
    int prob_result_accurate = probability_1_100(80*compass_navigation_accuracy_increase(p_player));
    int prob_result_offset_left0_right1 = probability_1_100(50);
    switch (direction){
        case UP:
            p_player->next_y = p_player->current_y - 1;
            if(prob_result_accurate) break;
            if (prob_result_offset_left0_right1) p_player->next_x = p_player->current_x + 1;
            else p_player->next_x = p_player->current_x - 1;
            break;
        case RIGHT:
            p_player->next_x = p_player->current_x + 1;
            if(prob_result_accurate) break;
            if (prob_result_offset_left0_right1) p_player->next_y = p_player->current_y + 1;
            else p_player->next_y = p_player->current_y - 1;
            break;
        case DOWN:
            p_player->next_y = p_player->current_y + 1;
            if(prob_result_accurate) break;
            if (prob_result_offset_left0_right1) p_player->next_x = p_player->current_x - 1;
            else p_player->next_x = p_player->current_x + 1;
            break;
        case LEFT:
            p_player->next_x = p_player->current_x - 1;
            if(prob_result_accurate) break;
            if (prob_result_offset_left0_right1) p_player->next_y = p_player->current_y - 1;
            else p_player->next_y = p_player->current_y + 1;
            break;
    }
}

void action(WINDOW* console_window, location **map, _player* p_player, element* elements){ /*Η συνάρτηση καλείται όταν ο παίκτης αλλάζει θέσει, βρίσκει το element-struct του στοιχείου
 * που υπάρχει στη νέα θέση του παίκτη (αν υπάρχει) και έπειτα καλεί την κατάλληλη συνάρτηση damage, heal ή inventory, ανάλογα με το "function" του στοιχείου.*/
    char temp_element_alias = map[p_player->next_y][p_player->next_x].elements[0];
    int i = 0;
    while (temp_element_alias != elements[i].alias){/*Εδώ βρίσκω το element-struct που αντιστοιχεί στο στοιχείου του αντικειμένου.*/
        i++;
    }

    if (elements[i].alias == 's'){ /*Εδώ αν ο παίκτης συναντάει καρχαρίες καλείται η shark_encounter και ο παίκτης επιστρέφει στην προηγούμενή του θέση (αν επιβιώσει).*/
        p_player->shark_counter++;
        wprintw(console_window, "\n %s\n", elements[i].text);
        shark_encounter(console_window, p_player);
        if(p_player->game_over == false){
            wprintw(console_window, " You managed to get back to shore.\n");
        }
        p_player->next_x = p_player->current_x;
        p_player->next_y = p_player->current_y;
    }
    else if (elements[i].function == 'd'){/*Εδώ καλείται η "damage" για τα στοιχεία που έχουν "function" 'd'*/
        damage(console_window, p_player, &elements[i]);
    }
    else if (elements[i].function == 'h'){/*Εδώ καλείται η "heal" για τα στοιχεία που έχουν "function" 'h'*/
        heal(console_window, p_player, &elements[i]);
    }
    else if (elements[i].function == 'w'){/*Εδώ καλείται η "win" για τα στοιχεία που έχουν "function" 'w', δηλαδή αν ο παίκτης βρει τον θησαυρό.*/
        wprintw(console_window, "\n %s\n", elements[i].text);
        win(console_window, p_player);
    }
    else if (elements[i].function == 'i'){/*Εδώ καλείται η "inventory" για τα στοιχεία που έχουν "function" 'i'*/
        inventory(console_window, map, p_player, &elements[i]);
    }

    p_player->current_x = p_player->next_x;
    p_player->current_y = p_player->next_y;
    if(p_player->game_over == false) {
        print_location(console_window, &map[p_player->current_y][p_player->current_x]);
        wrefresh(console_window);
    }
}

void inventory(WINDOW* console_window, location **map, _player* p_player, element* p_element){ /*Η συνάρτηση καλείται όταν βρει ο παίκτης κάποιο αντικείμενο.*/
    wprintw(console_window, "\n %s\n", p_element->text);
    for (int i = 0; i < INVENTORY_SIZE; i++) {/*Εδώ ψάχνει αν ο παίκτης έχει κάποια ελεύθερη θέση στο inventory. Αν έχει τοποθετεί το αντικείμενο εκεί.*/
        if(p_player->inventory[i]->name == NULL) {
            p_player->inventory[i] = p_element;
            map[p_player->next_y][p_player->next_x].elements[0] = '\0';
            return;
        }
    }
    /*Αν δεν υπάρχει κενή θέση, ρωτάει τον παίκτη αν θέλει να ανταλλάξει κάποιο από τα αντικείμενα που ήδη έχει για το καινούργιο.*/
    wprintw(console_window, "\n Your inventory is full!\n Do you want to switch an item that you already have for the %s?\n Press ESC to skip or ENTER to exchange an item...\n", p_element->name);
    wrefresh(console_window);
    int input = getch();
    switch (input){
        case 27:
            wprintw(console_window, "\n Pressed ESC\n");
            break;
        case '\n':
            wprintw(console_window, "\n Type the number of the inventory item that you want to exchange.\n", p_element->name);
            wrefresh(console_window);
            int input = getch();
            p_player->inventory[input-48-1] = p_element;
            map[p_player->next_y][p_player->next_x].elements[0] = '\0';
            break;
        default:
            wprintw(console_window, "\n Pressed invalid key! Continuing...\n");
            break;
    }
}

float damage_reduction(_player* p_player){/*Η συνάρτηση καλείται για να βρεθεί αν ο παίκτης έχει αντικείμενα στην κατοχή του που τον βοηθούν να αμυνθεί.
 * Σκανάρει θέση-θέση το inventory του παίκτη και αν υπάρχουν αντικείμενα που έχουν ως inventory_effect το "damage_reduction" μειώνει κατά το "inventory_effect_amount" το
 * "damage_reduction_value", το οποίο είναι ένα float που επιστρέφει η συνάρτηση.*/
    float damage_reduction_value = 1;
    for (int i = 0; i < INVENTORY_SIZE; i++) {
        if(p_player->inventory[i]!=NULL && strcmp(p_player->inventory[i]->inventory_effect, "damage_reduction") == 0){
            damage_reduction_value = damage_reduction_value - p_player->inventory[i]->inventory_effect_amount;
        }
    }
    if(damage_reduction_value<0) damage_reduction_value=0;
    return damage_reduction_value;
}

void damage(WINDOW* console_window, _player* p_player, element* p_element){/*Η συνάρτηση καλείται όταν ο παίκτης συναντά κάποιο στοιχείο που του αφαιρεί ζωή.*/
    p_player->health = p_player->health - p_element->function_amount*damage_reduction(p_player);/*Εδώ καλείται η damage_reduction για να βρεθεί αν ο παίκτης έχει
 * αντικείμενα που τον βοηθούν να επιβιώσει.*/
    wprintw(console_window, "\n %s\n", p_element->text);
    wrefresh(console_window);
    if(p_player->health <= 0){
        p_player->health = 0;
        death(console_window, p_player);
    }
}

void heal(WINDOW* console_window, _player* p_player, element* p_element){/*Η συνάρτηση καλείται όταν ο παίκτης συναντά κάποιο στοιχείο που του δίνει ζωή.*/
    p_player->health = p_player->health + p_element->function_amount;
    if(p_player->health > 10) p_player->health = 10;
    wprintw(console_window, "\n %s\n", (*p_element).text);
    wrefresh(console_window);
}

void death(WINDOW* console_window, _player* p_player){ /*Η συνάρτηση καλείται όταν το health του παίκτη γίνεται <= του μηδενός και όταν τον τρώνε οι καρχαρίες.*/
    wprintw(console_window, "\n\n You died! Better luck next time!\n\n");
    wrefresh(console_window);
    p_player->game_over = true;
}

void win(WINDOW* console_window, _player* p_player){ /*Η συνάρτηση καλείται όταν ο παίκτης βρίσκει τον θησαυρό.*/
    wprintw(console_window, "\n You won! Congratulations!\n");
    wrefresh(console_window);
    p_player->game_over = true;
}