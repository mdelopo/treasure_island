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
    system("@cls||clear");

    printf("  _______                                         _____       _                    _ \n"
           " |__   __|                                       |_   _|     | |                  | |\n"
           "    | | _ __  ___   __ _  ___  _   _  _ __  ___    | |   ___ | |  __ _  _ __    __| |\n"
           "    | || '__|/ _ \\ / _` |/ __|| | | || '__|/ _ \\   | |  / __|| | / _` || '_ \\  / _` |\n"
           "    | || |  |  __/| (_| |\\__ \\| |_| || |  |  __/  _| |_ \\__ \\| || (_| || | | || (_| |\n"
           "    |_||_|   \\___| \\__,_||___/ \\__,_||_|   \\___| |_____||___/|_| \\__,_||_| |_| \\__,_|\n");
    char intro_text[] = "\n    by Manolis Ntelopoulos AEM:10693 entelopo@ece.auth.gr"
                        "\n\nPlease read READ_ME.pdf before playing.\n";
    printf("%s\n", intro_text);

    _player player; //Δημιουργώ τον player
    char* map_filename = user_select_map_filename();//Ζητώ από τον χρήστη το όνομα του αρχείου-χάρτη
    char* elements_filename = user_select_elements_filename();//Ζητώ από τον χρήστη το όνομα του αρχείου-στοιχείων
    restart:;
    location **map = load_map_from_file(map_filename);//Φορτώνω τον χάρτη
    element *elements = load_elements_from_file(elements_filename);//Φορτώνω τα στοιχεία

    initialize_player(&player, map);//Αρχικοποιώ τον player

    //Εδώ διαμορφώνω το ncurses
    initscr();
    start_color();
    curs_set(0);
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    //Εδώ δημιουργώ τα παράθυρα του ncurses
    WINDOW *console_box = newwin(10, 10, 0, 0);
    WINDOW *console_window = newwin(10, 10, 0, 0);
    WINDOW *map_box = newwin(10, 10, 0, 0);
    WINDOW *map_window = newwin(10, 10, 0, 0);
    WINDOW *status_box = newwin(10, 10, 0, 0);
    WINDOW *status_window = newwin(10, 10, 0, 0);

    scrollok(console_window, TRUE);//εδώ ενεργοποιώ το scrolling για το παράθυρο της κονσόλας
    refresh();
    resize_window(console_box, console_window, map_box, map_window, status_box, status_window, map);

    wprintw(console_window, " Press r to resize terminal if needed\n\n _________________________________\n\n");
    wrefresh(console_window);

    init_pair(CYAN, COLOR_WHITE, COLOR_CYAN); //1 Εδώ δημιουργώ τα χρωματικά ζευγάρια για τους χαρακτήρες
    init_pair(GREEN, COLOR_WHITE, COLOR_GREEN); //2
    init_pair(WHITE, COLOR_BLACK, COLOR_WHITE); //3
    init_pair(YELLOW, COLOR_BLACK, COLOR_YELLOW); //4
    init_pair(BLACK, COLOR_BLACK, COLOR_BLACK); //5
    init_pair(HEALTH, COLOR_BLACK, COLOR_RED); //6

    wbkgd(map_window, COLOR_PAIR(CYAN)); //Χρώμα του φόντου του χάρτη
    wrefresh(map_window);

    while (1) {
        action(console_window, map, &player, elements);
        print_player_status(status_window, &player);
        print_map(map_window, map);
//        print_map_point(map_window, player.current_y, player.current_x, 5); /*Η συγκεκριμένη εντολή μπορεί να βγει απο σχόλιο για να εμφανίζει τη θέση*/
        /*του παίκτη στον χάρτη. (για testing)*/
        wrefresh(map_window);
        if (player.game_over == true) { //Eδώ τσεκάρω αν έχει αλλάξει η κατάσταση game_over σε true και αν έχει βγαίνω από το loop.
            wprintw(console_window, "\n Press ENTER to restart the game.\n");
            wrefresh(console_window);
            break;
        }
        int input = getch();
        switch (input) {
            case 'r':
                resize_window(console_box, console_window, map_box, map_window, status_box, status_window, map);
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
                break;
            default:
                wprintw(console_window, "\n That key doesn't do anything. (Check CAPS LOCK)\n\n");
                wrefresh(console_window);
                break;
        }
    }

    while (1) {//Εδώ περιμένω το ENTER από τον χρήστη για να κάνω restart.
        int input = getch();
        if (input == '\n') {
            //endwin();
            goto restart;
        }
    }

    return 0;
}
