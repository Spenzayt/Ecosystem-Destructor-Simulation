#include <iostream>
<<<<<<< Updated upstream
=======
#include <iomanip>
#include <cstdlib>
#include <chrono>
#include "Gobie.cpp"
>>>>>>> Stashed changes

using namespace std;

int main() {
<<<<<<< Updated upstream
    cout << "Hello World!\n";
    cout << "Hello World!\n";
=======
	Gobie gobie1;
	Coordinates dir = gobie1.GetDirectionVector({ 5, 0 }, { 0, 5});
	cout << std::setprecision(2) << dir.x << ", " << dir.y;
	
>>>>>>> Stashed changes
}