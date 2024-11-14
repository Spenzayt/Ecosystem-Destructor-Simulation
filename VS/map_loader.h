#ifndef MAP_LOADER_H
#define MAP_LOADER_H

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

enum Key {
    INVALID = -1, UP, DOWN, LEFT, RIGHT, SPACE = 32, ENTER = 13, Z = 'z', S = 's', A = 'a', B = 'b'
};


void startGame();
void displayMenu();
Key getKeyInput();
bool checkKonamiCode(const std::vector<Key>& inputs);


void displayGameMenuOptions(int choice, bool konamiCodeActivated);

bool gameOver;

#endif // MAP_LOADER_H
