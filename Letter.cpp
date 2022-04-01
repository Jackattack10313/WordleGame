#include "Letter.h"
using std::cout, std::endl;

Letter::Letter() {
	letter = '\0';
	letter_state = '\0';
}

Letter::Letter (char letter) {
	this->letter = toupper(letter);
	letter_state = STATE_GREY;
}

void Letter::update_state(char state) {
	letter_state = state;
}

void Letter::print_debugging() {
	cout << "Letter: " << letter << " | state: " << letter_state << endl;
}