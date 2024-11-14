#ifndef DISPLAY_FUNCTIONS_H
#define DISPLAY_FUNCTIONS_H

#include <string>

// Affiche le texte centr� dans la console
void centerText(const std::string text, bool endline, int add);

// Affiche un grand titre en ASCII
void displayTitle();

// Affiche un petit titre en ASCII
void displayLittleTitle();

// Efface l'�cran de la console
void clearScreen();

// D�finit la couleur du texte dans la console
void setColorText(int textColor);

// D�finit la couleur de fond du texte dans la console
void setColorBg(int bgColor);

#endif // DISPLAY_FUNCTIONS_H
