#include "Display.h"
#include "MainMenu.h"

#include <iostream>
#include "animals.cpp"

using namespace std;


int main() {
	displayMainMenu();
  
  Fish fishManager;
  fishManager.AddFish();
}