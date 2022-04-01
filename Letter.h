#ifndef LETTER_H
#define LETTER_H

#include <iostream>

const char STATE_GREY = 'g';
const char STATE_YELLOW = 'y';
const char STATE_GREEN = 'e';

class Letter {
	private:
		char letter = '\0'; // Letter initialized to null char
		char letter_state = '\0'; // State initialized to null char
	public:
		Letter();
		Letter(char letter);
		char get_state() {return letter_state;}; // Returns state
		void update_state(char state); // Changes state
		char get_character() {return letter;}; // Returns letter
		void print_debugging(); // Testing
};

#endif