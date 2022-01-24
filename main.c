#include "maputils.h"
#include "terminalutils.h"
#include "elementsutils.h"
#include "gameutils.h"
#include <curses.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>



int map_file_rows = 0, map_file_columns = 0;

enum colours {
    ZERO, CYAN, GREEN, WHITE, YELLOW, BLACK, HEALTH
};

enum directions {
    UP, RIGHT, DOWN, LEFT
};

int main() {

    _player player;
    char* map_filename = user_select_map_filename();
    char* elements_filename = user_select_elements_filename();
    restart:;
    location **map = load_map_from_file(map_filename);
    element *elements = load_elements_from_file(elements_filename);

    initialize_player(&player, map);
    //return 0;


    initscr();
    start_color();
    curs_set(0);
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    WINDOW *console_box = newwin(10, 10, 0, 0);
    WINDOW *console_window = newwin(10, 10, 0, 0);
    WINDOW *map_box = newwin(10, 10, 0, 0);
    WINDOW *map_window = newwin(10, 10, 0, 0);
    WINDOW *status_box = newwin(10, 10, 0, 0);
    WINDOW *status_window = newwin(10, 10, 0, 0);

    scrollok(console_window, TRUE);
    refresh();
    resize_window(console_box, console_window, map_box, map_window, status_box, status_window);
    wprintw(console_window, " Press m to load map\n\n\n Press r to resize terminal\n\n\n Press c to clear map-window\n\n _________________________________\n\n");
    wrefresh(console_window);

    init_pair(CYAN, COLOR_WHITE, COLOR_CYAN); //1
    init_pair(GREEN, COLOR_WHITE, COLOR_GREEN); //2
    init_pair(WHITE, COLOR_BLACK, COLOR_WHITE); //3
    init_pair(YELLOW, COLOR_BLACK, COLOR_YELLOW); //4
    init_pair(BLACK, COLOR_BLACK, COLOR_BLACK); //5
    init_pair(HEALTH, COLOR_BLACK, COLOR_RED); //6

    wbkgd(map_window, COLOR_PAIR(CYAN));
    wrefresh(map_window);

    print_map(map_window, map);
    print_map_point(map_window, player.current_y, player.current_x, BLACK);
    wrefresh(map_window);
    print_player_status(status_window, &player);
#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
    while (1) {
        int input = getch();
        switch (input) {
            case 'r':
                resize_window(console_box, console_window, map_box, map_window, status_box, status_window);
                break;
            case 's':
                print_player_status(status_window, &player);
                break;
            case 'c':
                werase(map_window);
                wrefresh(map_window);
                break;
            case 'm':
                print_map(map_window, map);
                break;
            case 'b':
            case KEY_UP:
            case 'n':
            case KEY_DOWN:
            case 'a':
            case KEY_RIGHT:
            case 'd':
            case KEY_LEFT:
                switch (input) {
                    case 'b':
                    case KEY_UP:
                        movement(&player, UP);
                        break;
                    case 'a':
                    case KEY_RIGHT:
                        movement(&player, RIGHT);
                        break;
                    case 'n':
                    case KEY_DOWN:
                        movement(&player, DOWN);
                        break;
                    case 'd':
                    case KEY_LEFT:
                        movement(&player, LEFT);
                        break;
                    default:
                        exit(EXIT_FAILURE);
                        break;
                }
                action(console_window, map, &player, elements);
                print_player_status(status_window, &player);
                print_map(map_window, map);
                print_map_point(map_window, player.current_y, player.current_x, 5);
                break;
            case 't':
                damage(console_window, &player, &elements[0]);
                print_player_status(status_window, &player);
            default:
                break;
        }
        wrefresh(map_window);
        if (player.game_over == true) {
            wprintw(console_window, "\n Press enter to restart the game.\n");
            wrefresh(console_window);
            break;
        }
    }
#pragma clang diagnostic pop
    while (1) {
        int input = getch();
        if (input == '\n') {
            endwin();
            goto restart;
        }
    }

    return 0;
}
