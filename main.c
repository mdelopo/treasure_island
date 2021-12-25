#include "maputlis.h"
#include <curses.h>

void draw_borders(WINDOW *screen) {
    int x, y, i;
    getmaxyx(screen, y, x);
    mvwprintw(screen, 0, 0, "+");
    mvwprintw(screen, y - 1, 0, "+");
    mvwprintw(screen, 0, x - 1, "+");
    mvwprintw(screen, y - 1, x - 1, "+");
    for (i = 1; i < (y - 1); i++) {
        mvwprintw(screen, i, 0, "|");
        mvwprintw(screen, i, x - 1, "|");
    }
    for (i = 1; i < (x - 1); i++) {
        mvwprintw(screen, 0, i, "-");
        mvwprintw(screen, y - 1, i, "-");
    }
}


int main() {
    location loc;
    read_location_from_file("island_default.csv", 5, 4, &loc);
    print_location(&loc);

    initscr();

    addstr("-----------------\n| codedrome.com |\n| ncurses Demo  |\n-----------------\n\n");
    refresh();

    //printing();

    // moving_and_sleeping();

    //colouring();
    WINDOW *score = newwin(20, 20, 0, 0);
    WINDOW *field = newwin(20, 20, 0, 0);

    int parent_x, parent_y = 0, new_x, new_y;
    int score_size = 3; // ...
    draw_borders(field);
    draw_borders(score);
    while(1){
        getmaxyx(stdscr, new_y, new_x);
        if (new_y != parent_y || new_x != parent_x) {
            parent_x = new_x; parent_y = new_y;
            wresize(field, new_y - score_size, new_x);
            wresize(score, score_size, new_x);
            mvwin(score, new_y - score_size, 0);
            wclear(stdscr);
            wclear(field);
            wclear(score);
            draw_borders(field);
            draw_borders(score);
        } // draw to our windows
        mvwprintw(field, 1, 1, "Field");
        mvwprintw(score, 1, 1, "Score"); // refresh each window
        wrefresh(field);
        wrefresh(score);
    } // ...

    endwin();

    return 0;
}