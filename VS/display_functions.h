#ifndef DISPLAY_FUNCTIONS_H
#define DISPLAY_FUNCTIONS_H

#include <string>

// Affiche le texte centré dans la console
void centerText(const std::string text, bool endline, int add);

// Affiche un grand titre en ASCII
void displayTitle();

// Affiche un petit titre en ASCII
void displayLittleTitle();

// Efface l'écran de la console
void clearScreen();

// Définit la couleur du texte dans la console
void setColorText(int textColor);

// Définit la couleur de fond du texte dans la console
void setColorBg(int bgColor);

#endif // DISPLAY_FUNCTIONS_H
