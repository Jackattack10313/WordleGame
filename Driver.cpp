#include "Game.h"

using std::cout;
using std::endl;
using std::vector;
using std::string;

Game game("words.dat", "frequencies.dat");

void menu() {
    cout << "Welcome to Wordle!" << endl;
    cout << "(1) Play yourself" << endl;
    cout << "(2) Let Computer Play" << endl;
    int selection;
    bool invalid = true;
    do {
        try {
            cout << "Enter selection: ";
            string temp;
            std::cin >> temp;
            selection = std::stoi(temp);
            if (selection > 0 && selection < 3) {
                invalid = false;
            }
        }
        catch (std::invalid_argument& e) {
            cout << "Enter a number!" << endl;
        }
    } while (invalid);
    if (selection == 1) {
        game.runGameHuman();
    }
    else {
        game.runGameComputer();
    }
}

int main() {
    menu();
    return 0;
}