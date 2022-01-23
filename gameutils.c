#include "gameutils.h"
#include "maputils.h"
#include "elementsutils.h"
#include <curses.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void initialize_player(_player* player, location** map){
    player->current_x = 2;
    player->current_y = 2;
    for(int x = 0; x<map_file_columns; x++){
        for(int y = 0; y<map_file_rows; y++){
            if(map[y][x].elements[0]=='@'){
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
int probability_1_100(int p){
    int num = rand()%(100+1);
    if (num <= p) return 1;
    else return 0;
}

int shark_encounter(WINDOW* console_window,  _player* p_player){
    int sharks_appetite[3] = {20, 70, 100};
    int result = probability_1_100(sharks_appetite[p_player->shark_counter - 1]);
    if(result == 1){
        p_player->health = 0;
        death(console_window, p_player);
    }
}
float compass_navigation_accuracy_increase(_player* p_player){
    float navigation_accuracy_increase_value = 1;
    for (int i = 0; i < INVENTORY_SIZE; i++) {
        if(p_player->inventory[i]!=NULL && strcmp(p_player->inventory[i]->inventory_effect, "navigation_accuracy_increase") == 0){
            navigation_accuracy_increase_value = navigation_accuracy_increase_value + p_player->inventory[i]->inventory_effect_amount;
        }
    }
    if(navigation_accuracy_increase_value<0) navigation_accuracy_increase_value=0;
    return navigation_accuracy_increase_value;
}

void movement(_player* p_player, int direction){
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

void action(WINDOW* console_window, location **map, _player* p_player, element* elements){
    char temp_element_alias = map[p_player->next_y][p_player->next_x].elements[0];
    int i = 0;
    while (temp_element_alias != elements[i].alias){
        i++;
    }

    if (elements[i].alias == 's'){
        p_player->shark_counter++;
        wprintw(console_window, "\n %s\n", elements[i].text);
        shark_encounter(console_window, p_player);
        if(p_player->game_over == false){
            wprintw(console_window, " You managed to get back to shore.\n");
        }
        p_player->next_x = p_player->current_x;
        p_player->next_y = p_player->current_y;
    }
    else if (elements[i].function == 'd'){
        damage(console_window, p_player, &elements[i]);
    }
    else if (elements[i].function == 'h'){
        heal(console_window, p_player, &elements[i]);
    }
    else if (elements[i].function == 'w'){
        wprintw(console_window, "\n %s\n", elements[i].text);
        win(console_window, p_player);
    }
    else if (elements[i].function == 'i'){
        inventory(console_window, map, p_player, &elements[i]);
    }

    p_player->current_x = p_player->next_x;
    p_player->current_y = p_player->next_y;
    if(p_player->game_over == false) {
        print_location(console_window, &map[p_player->current_y][p_player->current_x]);
        wrefresh(console_window);
    }
}

void inventory(WINDOW* console_window, location **map, _player* p_player, element* p_element){
    wprintw(console_window, "\n %s\n", p_element->text);
    for (int i = 0; i < INVENTORY_SIZE; i++) {
        if(p_player->inventory[i]->name == NULL) {
            p_player->inventory[i] = p_element;
            map[p_player->next_y][p_player->next_x].elements[0] = '\0';
            return;
        }
    }
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

float damage_reduction(_player* p_player){
    float damage_reduction_value = 1;
    for (int i = 0; i < INVENTORY_SIZE; i++) {
        if(p_player->inventory[i]!=NULL && strcmp(p_player->inventory[i]->inventory_effect, "damage_reduction") == 0){
            damage_reduction_value = damage_reduction_value - p_player->inventory[i]->inventory_effect_amount;
        }
    }
    if(damage_reduction_value<0) damage_reduction_value=0;
    return damage_reduction_value;
}

void damage(WINDOW* console_window, _player* p_player, element* p_element){
    p_player->health = p_player->health - p_element->function_amount*damage_reduction(p_player);
    wprintw(console_window, "\n %s\n", p_element->text);
    wrefresh(console_window);
    if(p_player->health <= 0){
        p_player->health = 0;
        death(console_window, p_player);
    }
}

void heal(WINDOW* console_window, _player* p_player, element* p_element){
    p_player->health = p_player->health + p_element->function_amount;
    if(p_player->health > 10) p_player->health = 10;
    wprintw(console_window, "\n %s\n", (*p_element).text);
    wrefresh(console_window);
}

void death(WINDOW* console_window, _player* p_player){
    wprintw(console_window, "\n\n You died! Better luck next time!\n\n");
    wrefresh(console_window);
    p_player->game_over = true;
}

void win(WINDOW* console_window, _player* p_player){
    wprintw(console_window, "\n You won! Congratulations!\n");
    wrefresh(console_window);
    p_player->game_over = true;
}