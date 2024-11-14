#ifndef GAME_H
#define GAME_H

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

extern bool gameOver;
extern Map map;

// �num�ration pour les touches de commande dans le jeu
enum Key {
    INVALID = -1, UP, DOWN, LEFT, RIGHT, SPACE = 32, ENTER = 13, Z = 'z', S = 's', A = 'a', B = 'b'
};

// Obtient l'entr�e de l'utilisateur pour les commandes du jeu
Key getKeyInput();

// V�rifie si la s�quence de touches saisie correspond au code Konami
bool checkKonamiCode(const std::vector<Key>& inputs);

// Affiche les options du menu de jeu, avec une option de triche si le code Konami est activ�
void displayGameMenuOptions(const int choice, bool konamiCodeActivated);

// G�re le menu principal du jeu et ses interactions
void displayMenu();

// Lance et g�re la boucle principale du jeu
void startGame();

#endif // GAME_H
