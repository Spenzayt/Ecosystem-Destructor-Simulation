#include "display_functions.h"
#include <iostream>
#include <windows.h>

using namespace std;


void centerText(const string text, bool endline, int add) {
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	int consoleWidth;

	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	consoleWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;

	int padding = (consoleWidth - (text.length() + add)) / 2;

	cout << string(padding > 0 ? padding : 0, ' ') << text;
	if (endline) {
		cout << endl;
	}
}

void displayTitle() {
	centerText(" _____                                                                            _____ ", true, 0);
	centerText("( ___ )                                                                          ( ___ )", true, 0);
	centerText(" |   |~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|   | ", true, 0);
	centerText(" |   |                                                                            |   | ", true, 0);
	centerText(" |   |           ____            __                  __                           |   | ", true, 0);
	centerText(" |   |          / __ \\___  _____/ /________  _______/ /____  __  _______          |   | ", true, 0);
	centerText(" |   |         / / / / _ \\/ ___/ __/ ___/ / / / ___/ __/ _ \\/ / / / ___/          |   | ", true, 0);
	centerText(" |   |        / /_/ /  __(__  / /_/ /  / /_/ / /__/ /_/  __/ /_/ / /              |   | ", true, 0);
	centerText(" |   |       /_____/\\___/____/\\__/_/   \\__,_/\\___/\\__/\\___/\\__,_/_/               |   | ", true, 0);
	centerText(" |   |    ____/ ( )___  _________  _______  _______/ /____  ____ ___  ___  _____  |   | ", true, 0);
	centerText(" |   |   / __  /|// _ \\/ ___/ __ \\/ ___/ / / / ___/ __/ _ \\/ __ `__ \\/ _ \\/ ___/  |   | ", true, 0);
	centerText(" |   |  / /_/ /  /  __/ /__/ /_/ (__  / /_/ (__  / /_/  __/ / / / / /  __(__  )   |   | ", true, 0);
	centerText(" |   |  \\__,_/   \\___/\\___/\\____/____/\\__, /____/\\__/\\___/_/ /_/ /_/\\___/____/    |   | ", true, 0);
	centerText(" |   |                               /____/                                       |   | ", true, 0);
	centerText(" |   |                                                                            |   |", true, 0);
	centerText(" |___|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|___| ", true, 0);
	centerText("(_____)                                                                          (_____)", true, 0);
}


void clearScreen() {
	system("cls");
}

void setColor(int textColor) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	int color = textColor;
	SetConsoleTextAttribute(hConsole, color);
}
