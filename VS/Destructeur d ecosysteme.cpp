#include "Display.h"
#include "MainMenu.h"

#include <iostream>
#include "animals.cpp"
#include <cstdlib>
#include <chrono>

using namespace std;


int main() {
<<<<<<< Updated upstream
	displayMainMenu();
  
  Fish fishManager;
  fishManager.AddFish();
=======
    
    SpecieManager fishManager("Poisson");


    //fishManager.AddAnimal();
    //fishManager.AddAnimal();
    //fishManager.ShowCoords();
    srand(time(NULL));
    cout << rand() % 10;
>>>>>>> Stashed changes
}