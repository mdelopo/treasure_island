#include "terminalutils.h"
#include "maputils.h"
#include "gameutils.h"

#include <curses.h>

void resize_window(WINDOW* console_box, WINDOW* console_window, WINDOW* map_box, WINDOW* map_window, WINDOW* status_box, WINDOW* status_window){
    clear();
    int mx, my;
    getmaxyx(stdscr, my, mx);
    int console_box_x_size = (int)(0.66*mx);
    int console_box_y_size = my;
    int map_box_start_x_coordinate = console_box_x_size;
    int map_box_x_size = mx - console_box_x_size;
    int map_box_y_size = (int)(0.6*my);
    int status_box_x_size = map_box_x_size;
    int status_box_y_size = my-map_box_y_size;
    wresize(console_box, console_box_y_size, console_box_x_size);
    mvwin(console_box, 0, 0);
    wresize(console_window, console_box_y_size-2, console_box_x_size-2);
    mvwin(console_window, 1, 1);
    wresize(map_box, map_box_y_size, map_box_x_size);
    mvwin(map_box, 0, map_box_start_x_coordinate);
    wresize(map_window, map_box_y_size-2, map_box_x_size-2);
    mvwin(map_window, 1, map_box_start_x_coordinate + 1);
    wresize(status_box, status_box_y_size, map_box_x_size);
    mvwin(status_box, map_box_y_size, map_box_start_x_coordinate);
    wresize(status_window, status_box_y_size-2, status_box_x_size-2);
    mvwin(status_window, map_box_y_size + 1, map_box_start_x_coordinate + 1);
    refresh();
    box(console_box, 0, 0);
    box(map_box, 0, 0);
    box(status_box, 0, 0);
    wrefresh(console_box);
    wrefresh(console_window);
    wrefresh(map_box);
    wrefresh(map_window);
    wrefresh(status_box);
    wrefresh(status_window);
}

void print_map_point(WINDOW* map_window, int i, int j, int color_pair_num){
    for (int k = 0; k < 2; ++k) {
        for (int l = 0; l < 2; ++l) {
            wattron(map_window,COLOR_PAIR(color_pair_num));
            mvwaddch(map_window, i*2+k, j*2+l, 32);
            //wrefresh(map_window);
            wattroff(map_window,COLOR_PAIR(color_pair_num));
        }
    }
}
void print_map(WINDOW* map_window, location **map){
    wclear(map_window);
    int color_pair_num;
    for (int i = 0; i < map_file_rows; i++) {
        for (int j = 0; j < map_file_columns; j++) {
            switch (map[i][j].biome){
                case 'S':
                    color_pair_num = 1;
                    print_map_point(map_window, i ,j, 1);
                    break;
                case 'F':
                    color_pair_num = 2;
                    print_map_point(map_window, i ,j, 2);
                    break;
                case 'M':
                    color_pair_num = 3;
                    print_map_point(map_window, i ,j, 3);
                    break;
                case 'B':
                    color_pair_num = 4;
                    print_map_point(map_window, i ,j, 4);
                    break;
                default:
                    break;
            }

        }
    }
}

void print_player_status(WINDOW* status_window, _player* p_player){
    werase(status_window);
    mvwprintw(status_window,0,1,"Health:");
    wattron(status_window,COLOR_PAIR(6));
    for (int i = 1; i <= 2*p_player->health; i++) {
        mvwaddch(status_window, 1, i, 32);
    }
    wattroff(status_window,COLOR_PAIR(6));
    mvwprintw(status_window,1,22,"%.0f%%", p_player->health*10);

    wprintw(status_window,"\n\n Inventory:\n");
    for (int i = 0; i < INVENTORY_SIZE; i++) {
        if(p_player->inventory[i]->name != NULL) {
            wprintw(status_window, " %d. %s\n", i+1, p_player->inventory[i]->name);
        }
        else wprintw(status_window, "\n");
    }
    wrefresh(status_window);
}