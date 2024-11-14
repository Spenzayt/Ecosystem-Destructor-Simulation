#ifndef DISPLAY_FUNCTIONS_H
#define DISPLAY_FUNCTIONS_H

#include <iostream>
using namespace std;


void centerText(const string text, bool endline, int add);
void displayTitle();
void clearScreen();
void setColorText(int textColor);
void setColorBg(int bgColor);


#endif