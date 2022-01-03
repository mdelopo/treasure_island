#include "maputils.h"
#include "terminalutils.h"
#include "elementsutils.h"
#include <curses.h>
#include <stdlib.h>

int map_file_rows = 0, map_file_columns = 0;

typedef struct {
    char current_biome;
    float health;
    int current_x;
    int next_x;
    int current_y;
    int next_y;
    bool has_treasure;
} _player;

int main() {
    _player player;
    player.current_x = 2;
    player.current_y = 2;
    //location loc;
    //read_location_from_file("island_default.csv", 9, 4, &loc);
    //print_location(&loc);
    load_elements_from_file("elements_default.csv");
    location** map = load_map_from_file("island_default.csv");
    //printf("Rows: %d Columns: %d", file_rows, file_columns);

    initscr();
    start_color();
    curs_set(0);
    //cbreak();
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
    wprintw(console_window, " Press m to load map\n\n Press r to resize terminal\n\n Press c to clear map-window\n");
    wrefresh(console_window);

    init_pair(1,COLOR_WHITE, COLOR_CYAN);
    init_pair(2,COLOR_WHITE, COLOR_GREEN);
    init_pair(3,COLOR_BLACK, COLOR_WHITE);
    init_pair(4,COLOR_BLACK, COLOR_YELLOW);
    init_pair(5,COLOR_BLACK, COLOR_BLACK);

    wbkgd(map_window, COLOR_PAIR(1));
    wrefresh(map_window);

    print_map(map_window, map);
    print_map_point(map_window, player.current_y, player.current_x,5);
    wrefresh(map_window);
#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
    while(1){
    int input = getch();
    wprintw(status_window, "%c ", input);
    wrefresh(status_window);
        switch (input){
            case 'r':
                resize_window(console_box, console_window, map_box, map_window, status_box, status_window);
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
                        player.current_y--;
                        break;
                    case 'd':
                    case KEY_LEFT:
                        player.current_x--;
                        break;
                    case 'n':
                    case KEY_DOWN:
                        player.current_y++;
                        break;
                    case 'a':
                    case KEY_RIGHT:
                        player.current_x++;
                        break;
                    default:
                        exit(EXIT_FAILURE);
                        break;
                }
                print_map(map_window, map);
                print_map_point(map_window, player.current_y, player.current_x,5);
                break;
            default:
                break;
        }
        wrefresh(map_window);
        print_location(console_window, &map[player.current_y][player.current_x]);
    }
#pragma clang diagnostic pop

    endwin();

    return 0;
}