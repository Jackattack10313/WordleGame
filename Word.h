#ifndef WORD_H
#define WORD_H

#include <string>
#include <vector>
#include <stdexcept>
#include <fstream>
#include <time.h>
#include <stdlib.h>
#include <cctype>
#include "Letter.h"

const int WORD_LENGTH = 5;

class Word {
	private:
		std::string secret_word = ""; // String storing the secret word
		Letter alphabet[26]; // Stores states of all letters in alphabet
		std::vector<std::string> allowed_guesses;
	public:
		Word(std::string answers_file, std::string guesses_file); // Randomly choosing word from file for secret word
		void take_guess(std::string guess); // Looping through word, printing the colored letters & updating alphabet
		bool win_state(); // Check to see if game is won
		void print(); // Print alphabet (colored)
		void print_debugging(); // Testing
};

#endif