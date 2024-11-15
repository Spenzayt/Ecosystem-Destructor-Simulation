#include "map_loader.h"
#include "display_functions.h"
#include "map_loader.h"

#include <iostream>
#include <conio.h>
#include <cctype>

enum MenuOption { PLAY = 0, EXIT };


void displayMenuOptions(const int choice) {
    clearScreen();
    displayTitle();

    centerText("========================================", true, 0);
    centerText("=               Main Menu              =", true, 0);
    centerText("========================================", true, 0);
    centerText((choice == PLAY ? "> Play" : "  Play"), true, 0);
    centerText((choice == EXIT ? "> Exit" : "  Exit"), true, 0);
    centerText("========================================", true, 0);
}


int displayMainMenu() {
    int choice = PLAY;

    while (true) {
        displayMenuOptions(choice);

        char input = _getch();

        switch (tolower(input)) {
        case 'z':
            choice = PLAY;
            break;
        case 's':
            choice = EXIT;
            break;
        case ' ': case 13:
            if (choice == PLAY) {
                startGame();
                return PLAY;
            }
            else if (choice == EXIT) {
                return EXIT;
            }
            break;
        default:
            break;
        }
    }
}