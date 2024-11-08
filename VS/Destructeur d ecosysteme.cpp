#include <iostream>
#include "animals.cpp"

using namespace std;

int main() {
    cout << "Hello World!\n";
    cout << "Hello World!\n";
    cout << "Hello World!\n";
    Animals test;
    cout << test.getCoord().x << test.getCoord().y;
}