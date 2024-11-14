#ifndef MENU_H
#define MENU_H

#include "map_loader.h"
#include "display_functions.h"
#include <iostream>
#include <conio.h>
#include <cctype>

enum MenuOption { PLAY = 0, EXIT };

// Affiche les options du menu principal, avec la sélection actuelle en surbrillance
void displayMenuOptions(const int choice);

// Affiche le menu principal et gère les interactions utilisateur
int displayMainMenu();

#endif // MENU_H
