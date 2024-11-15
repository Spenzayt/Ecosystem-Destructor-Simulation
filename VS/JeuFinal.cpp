#include <iostream>
#include <cstdlib>
#include <ctime>
#include <windows.h>
#include <conio.h>
#include <cctype>
#include <chrono>
#include <time.h>
#include <vector>
#include <string>
#include <thread>
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
int tournb = 1;

class Animals {
protected:
    int x, y;
    string type;

public:
    int food = 100;
    bool live;

    Animals(int food, string type, bool live) : food(food), x(rand() % 5), y(rand() % 5), type(type) , live(live){}

    virtual void manger(Animals* autre) = 0;
    /*virtual void suicide(Animals) = 0;*/

    bool alive() const {
        return live;
    }

    double distanceAvec(const Animals* autre) const {
        return sqrt(pow(x - autre->x, 2) + pow(y - autre->y, 2));
    }

    void move(int dx, int dy) {
        x = max(0, min(4, x + dx));
        y = max(0, min(4, y + dy));
        cout << type << " se déplace en (" << x << ", " << y << ").\n";
    }

    string getType() const {
        return type;
    }

    void getInfo() const {
        cout << "Coordonnées : (" << x << ", " << y << "), Nourriture : " << food << ", Type : " << type << ", Est en vie? " << live << endl;
    }
};

class Loup : public Animals {
public:
    Loup() : Animals(rand() % 100, "Loup", true) {}

    void manger(Animals* autre)  {
        if (autre && distanceAvec(autre) == 0 && autre->getType() == "Ours") {
            autre->live = false;
            food += 10;
            cout << autre->getType() << " a été mangé par " << type << "!\n";
        }
    }
    void suicide() {

    }
};

class Ours : public Animals {
public:
    Ours() : Animals(rand() % 100, "Ours", true) {}

    void manger(Animals* autre)  {
        if (autre && distanceAvec(autre) == 0 && autre->getType() == "Loup") {
            autre->live = false;
            food += 10;
            cout << autre->getType() << " a été mangé par " << type << "!\n";
        }
    }
};
class Aigle : public Animals {
public:
    Aigle() : Animals(rand() % 100, "Aigle", true) {}

    void manger(Animals* autre)  {
        if (autre && distanceAvec(autre) == 0 && autre->getType() == "Loup") {
            autre->live = false;
            food += 10;
            cout << autre->getType() << " a été mangé par " << type << "!\n";
        }
    }
};
class Elephant : public Animals {
public:
    Elephant() : Animals(rand() % 100, "Elephant", true) {}

    void manger(Animals* autre) override {
        if (autre && distanceAvec(autre) == 0 && autre->getType() == "Loup") {
            autre->live = false;
            food += 10;
            cout << autre->getType() << " a été mangé par " << type << "!\n";
        }
    }
};

class Jeu {
private:
    vector<Animals*> animals;

public:
    ~Jeu() {
        for (auto animal : animals) {
            delete animal;
        }
        cout << "Tous les animaux ont été détruits.\n";
    }

    void init(int nombrePersonnages) {
        srand(time(0));
        for (int i = 0; i < nombrePersonnages; ++i) {
            int classe = rand() % 4;
            switch (classe) {
            case 0: animals.push_back(new Ours()); break;
            case 1: animals.push_back(new Loup()); break;
            case 2: animals.push_back(new Aigle()); break;
            case 3: animals.push_back(new Elephant()); break;
            };
        }
    }

    void tourDeJeu() {
        for (auto animal : animals) {
            if (!animal->alive()) continue;

            int action = rand() % 2;
            switch (action) {
            case 0: // Déplacement
                animal->move(rand() % 3 - 1, rand() % 3 - 1);
                break;
            case 1: // Manger
                for (auto& cible : animals) {
                    if (cible != animal && cible->alive()) {
                        animal->manger(cible);
                        break;
                    }
                }
                break;
            }
        }

        // Supprimer les animaux morts
        animals.erase(
            remove_if(animals.begin(), animals.end(),
                [](Animals* animal) {
                    if (!animal->alive()) {
                        delete animal;
                        return true;
                    }
                    return false;
                }),
            animals.end()
        );
    }

    bool verifierVictoire() {
        string especeRestante;
        bool premiereEspeceTrouvee = false;

        for (const auto& animal : animals) {
            if (!animal->alive()) continue;

            if (!premiereEspeceTrouvee) {
                especeRestante = animal->getType();
                premiereEspeceTrouvee = true;
            }
            else if (animal->getType() != especeRestante) {
                return false; // Plus d'une espèce vivante trouvée
            }
        }

        return true; // Une seule espèce vivante reste
    }

    void checkEspece(const string& espece) {
        int count = 0;
        for (const auto& animal : animals) {
            if (animal->alive() && animal->getType() == espece) {
                animal->getInfo();
                ++count;
            }
        }
        if (count == 0) {
            cout << "Aucun animal de type " << espece << " n'a été trouvé.\n";
        }
    }

    void menu() {
        while (true) {
            cout << "Que voulez-vous faire ?\n";
            cout << "1. Passer au jour suivant\n";
            cout << "2. Consulter les informations sur une espèce\n";
            cout << "3. Quitter le jeu\n";
            int choix;
            cin >> choix;

            if (choix == 1) {
                return;
            }
            else if (choix == 2) {
                cout << "Entrez le nom de l'espèce (Ours, Loup, Elephant, Aigle) : ";
                string espece;
                cin >> espece;
                checkEspece(espece);
            }
            else if (choix == 3) {
                cout << "Vous avez quitté le jeu.\n";
                exit(0);
            }
            else {
                cout << "Choix invalide. Veuillez réessayer.\n";
            }
        }
    }

    void lancerJeu(int nmbAnimals) {
        init(nmbAnimals);
        while (!verifierVictoire()) {
            menu();
            tourDeJeu();
            ++tournb;
        }

        string especeGagnante;
        for (const auto& animal : animals) {
            if (animal->alive()) {
                especeGagnante = animal->getType();
                break;
            }
        }

        cout << "L'espèce " << especeGagnante << " a dominé la bataille royale!\n";
    }
};

Jeu jeu;








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
#include <iostream>
#include <conio.h>
#include <cctype>

enum MenuOption { PLAY = 0, EXIT };


void displayMenuOptions(const int choice) {
    clearScreen();
    displayTitle();

    centerText("========================================", true, 0);
    centerText("=               Main Menu              =", true, 0);
    centerText("========================================", true, 0);
    centerText((choice == PLAY ? "> Play" : "  Play"), true, 0);
    centerText((choice == EXIT ? "> Exit" : "  Exit"), true, 0);
    centerText("========================================", true, 0);
}


int displayMainMenu() {
    int choice = PLAY;

    while (true) {
        displayMenuOptions(choice);

        char input = _getch();

        switch (tolower(input)) {
        case 'z':
            choice = PLAY;
            break;
        case 's':
            choice = EXIT;
            break;
        case ' ': case 13:
            if (choice == PLAY) {
                int nmbanimal;
                cout << "Combien d'animaux? : ";
                cin >> nmbanimal;
                jeu.lancerJeu(nmbanimal);
                return PLAY;
            }
            else if (choice == EXIT) {
                return EXIT;
            }
            break;
        default:
            break;
        }
    }
}


int main() {
    
    
    displayMainMenu();
    return 0;
}
