#include "gameutils.h"
#include "maputils.h"
#include "elementsutils.h"
#include <curses.h>
//#include <string.h>
#include <stdlib.h>

void initialize_player(_player* player){
    player->current_x = 2;
    player->current_y = 2;
    player->next_x = player->current_x;
    player->next_y = player->current_y;
    player->health = 10;
    player->game_over = false;
    player->shark_counter = 0;
}

void action(WINDOW* console_window, location **map, _player* p_player, element* elements) {
    char temp_element_alias = map[p_player->next_y][p_player->next_x].elements[0];
    int i = 0;
    while (temp_element_alias != elements[i].alias){
        i++;
    }

    if (elements[i].alias == 's'){
        wprintw(console_window, "\n %s\n", elements[i].text);
        p_player->shark_counter++;
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
    else{
        wprintw(console_window, " %s", elements[i].text);
    }
    p_player->current_x = p_player->next_x;
    p_player->current_y = p_player->next_y;
    if(p_player->game_over == false) {
        print_location(console_window, &map[p_player->current_y][p_player->current_x]);
        wrefresh(console_window);
    }
}

void damage(WINDOW* console_window, _player* p_player, element* p_element){
    p_player->health = p_player->health - p_element->function_amount;
    wprintw(console_window, "\n %s\n", p_element->text);
    wrefresh(console_window);
    if(p_player->health <= 0){
        p_player->health = 0;
        death(console_window, p_player);
    }
}

void heal(WINDOW* console_window, _player* p_player, element* p_element) {
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