#include "Animals.h"
#include "display_functions.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <windows.h>
#include <conio.h>
#include <vector>
#include <string>
#include <thread>
#include <chrono>

bool gameOver = false;
Map map;
enum Key {
    INVALID = -1, UP, DOWN, LEFT, RIGHT, SPACE = 32, ENTER = 13, Z = 'z', S = 's', A = 'a', B = 'b'
};
Key getKeyInput() {
    int ch = _getch();
    clearScreen();
    displayLittleTitle();
    map.displayMap();

    if (ch == 0 || ch == 224) {
        ch = _getch();
        switch (ch) {
        case 72: return UP;
        case 80: return DOWN;
        case 77: return RIGHT;
        case 75: return LEFT;
        }
    }

    if (ch == ' ') return SPACE;
    if (ch == ENTER) return ENTER;
    if (ch == 'z' || ch == 'Z') return Z;
    if (ch == 's' || ch == 'S') return S;
    if (ch == 'b' || ch == 'B') return B;
    if (ch == 'a' || ch == 'A') return A;

    return INVALID;
}

bool checkKonamiCode(const vector<Key>& inputs) {
    const vector<Key> konamiCode = { UP, UP, DOWN, DOWN, LEFT, RIGHT, LEFT, RIGHT, B, A };
    if (inputs.size() < konamiCode.size()) {
        return false;
    }

    for (size_t i = 0; i < konamiCode.size(); ++i) {
        if (inputs[inputs.size() - konamiCode.size() + i] != konamiCode[i]) {
            return false;
        }
    }
    return true;
}


void displayGameMenuOptions(const int choice, bool konamiCodeActivated) {
    cout << endl << endl;
    centerText("Day " + to_string(map.days), true, 0);
    centerText("========================================", true, 0);
    centerText((choice == 1 ? ">  1 - Pass a day" : "   1 - Pass a day"), true, 0);
    centerText((choice == 2 ? ">  2 - Check a tile" : "   2 - Check a tile"), true, 0);
    centerText((choice == 3 ? ">  3 - Quit" : "   3 - Quit"), true, 0);


    if (konamiCodeActivated) {
        cout << endl;
        centerText("       Cheat Command : ", true, 0);
        centerText((choice == 4 ? ">  4 - Flood map" : "   4 - Flood map"), true, 0);
        centerText((choice == 5 ? ">  5 - Wake volcano" : "   5 - Wake volcano"), true, 0);
        centerText((choice == 6 ? ">  6 - Nuke" : "   6 - Nuke"), true, 0);
    }
    centerText("========================================", true, 0);
}


void displayMenu() {
    int choice = 1;
    bool konamiCodeActivated = false;
    vector<Key> inputs;

    while (true) {
        displayGameMenuOptions(choice, konamiCodeActivated);
        Key input = getKeyInput();

        if (input != INVALID) {
            inputs.push_back(input);

            if (checkKonamiCode(inputs)) {
                konamiCodeActivated = true;
                inputs.clear();
            }
        }

        if (input == UP || input == Z){
            if (choice > 1) {
                choice--;
            }
        }
        else if (input == DOWN || input == S) {
            if (choice < (konamiCodeActivated ? 7 : 3)) {
                choice++;
            }
        }
        else if (input == SPACE || input == ENTER) {
            if (choice == 1) {
                map.nextDay();
            }
            else if (choice == 2) {
                map.checkTile();
            }
            else if (choice == 3) {
                gameOver = true;
                return;
            }
            else if (choice == 4) {
                map.StartFlooding();
                map.nextDay();
            }
            else if (choice == 5) {
                map.generateVolcano();
                map.nextDay();
            }
            else if (choice == 6) {
                map.nuke(15,15,16);
            }
        }
    }
    }

void startGame() {
    clearScreen();
    srand((unsigned)time(NULL));
    map.startGeneration();

    while (!gameOver) {
        displayMenu();
    }
}
