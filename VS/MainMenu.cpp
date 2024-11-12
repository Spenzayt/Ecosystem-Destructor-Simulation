#include "Display.h"

#include <iostream>
#include <conio.h>


// Display the Main Menu
int displayMainMenu() {
	int choice = 0;

	while (true) {
		clearScreen();
		displayTitle();

		centerText("========================================", true, 0);
		centerText("=               Main Menu              =", true, 0);
		centerText("========================================", true, 0);
		if (choice == 0) {
			centerText("> Play", true, 0);
			centerText("  Exit", true, 0);
		}
		else if (choice == 1) {
			centerText("  Play", true, 0);
			centerText("> Exit", true, 0);
		}
		centerText("========================================", true, 0);

		// Input
		char input = _getch();

		if (input == 'z' or input == 'Z') {
			choice = 0;
		}
		else if (input == 's' or input == 'S') {
			choice = 1;
		}
		else if (input == ' ' or input == 13) {
			if (choice == 0) {
				//Call Start Function
			}
			else if (choice == 1) {
				exit(0);
			}
		}
	}
}