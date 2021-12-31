#include "maputils.h"
#include "terminalutils.h"
#include <curses.h>

int file_rows = 0, file_columns = 0;

int main() {
    //location loc;
    //read_location_from_file("island_default.csv", 9, 4, &loc);
    //print_location(&loc);
    location** map = load_map_from_file("island_default.csv");
    //printf("Rows: %d Columns: %d", file_rows, file_columns);

    initscr();
    start_color();
    curs_set(0);
    cbreak();
    noecho();

    WINDOW *console_box = newwin(10, 10, 0, 0);
    WINDOW *console_window = newwin(10, 10, 0, 0);
    WINDOW *map_box = newwin(10, 10, 0,0);
    WINDOW *map_window = newwin(10, 10, 0, 0);

    scrollok(console_window, TRUE);
    refresh();
    resize_window(console_box, console_window, map_box, map_window);
    wprintw(console_window, " Press m to load map\n\n Press r to resize terminal\n\n Press c to clear map-window\n");
    wrefresh(console_window);
    //wprintw(map_window, "Screen window is created by calling newwin(0,0,0,0).");
    wrefresh(map_window);

    init_pair(1,COLOR_WHITE, COLOR_CYAN);
    init_pair(2,COLOR_WHITE, COLOR_GREEN);
    init_pair(3,COLOR_BLACK, COLOR_WHITE);
    init_pair(4,COLOR_BLACK, COLOR_YELLOW);

    wbkgd(map_window, COLOR_PAIR(1));
    wrefresh(map_window);

    //print_map(map_window, map);

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
    while(1){
    char input = getch();
        switch (input){
            case 'r':
                resize_window(console_box, console_window, map_box, map_window);
                break;
            case 'c':
                werase(map_window);
                wrefresh(map_window);
                break;
            case 'm':
                print_map(map_window, map);
                break;
            default:
                break;
        }
    }
#pragma clang diagnostic pop

    endwin();

    return 0;
}