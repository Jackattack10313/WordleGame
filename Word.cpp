#include "Word.h"
using std::fstream;
using std::vector;
using std::string;
using std::cout, std::endl;
using std::invalid_argument;

Word::Word(string answers_file, string guesses_file) {
	// Assigning each element in alphabet with each letter in the alphabet
	for (unsigned int i = 0; i < 26; i++) {
		alphabet[i] = Letter(static_cast<char>(65 + i));
	}
	
	// Seeding rand() with the current time
	srand(time(0));
	// Vector of all possible wordle words;
	vector<string> word_list;
	// Filestream declarations & validations that they successfully opened the answers_file & guesses_file
	fstream input1(answers_file);
	fstream input2(guesses_file);
	if (!input1.is_open() || !input2.is_open()) {
		throw invalid_argument("Invalid filenames!");
	}
	
	// Loading word_list with all words in file
	string curr_word;
	while(input1 >> curr_word) {
		string temp_word = "";
		for (unsigned int i = 0; i < curr_word.length(); i++) {
			temp_word.push_back(toupper(curr_word.at(i)));
		}
		word_list.push_back(temp_word);
	}

	// Loading allowed_guesses data member from file;
	while(input2 >> curr_word) {
		string temp_word = "";
		for (unsigned int i = 0; i < curr_word.length(); i++) {
			temp_word.push_back(toupper(curr_word.at(i)));
		}
		allowed_guesses.push_back(temp_word);
	}

	// Validation that word_list.size() != 0 -> prevents divide by 0
	if (word_list.size() != 0) {
		// Choosing a random secret_word
		secret_word = word_list.at(rand() % word_list.size());
	}
	else {
		throw invalid_argument("No words read from file!");
	}
}

void Word::take_guess(string guess) {
	// Validating that the length of the word is WORD_LENGTH
	if (guess.length() != WORD_LENGTH) {
		throw invalid_argument("Wrong guess length!");
	}
	
	// Converting to uppercase
	for (unsigned int i = 0; i < WORD_LENGTH; i++) {
		guess.at(i) = toupper(guess.at(i));
	}
	
	// Validating that the word is in allowed_guesses (also verifies that the characters are in alphabet)
	bool guess_found = false;
	for (unsigned int i = 0; i < allowed_guesses.size(); i++) {
		if (allowed_guesses.at(i) == guess) {
			guess_found = true;
		}
	}
	if (!guess_found) {
		throw invalid_argument("Invalid word!");
	}
	
	// Creating a copy of the secret word to remove character when found
	string secret_word_copy = "";
	for (unsigned int i = 0; i < WORD_LENGTH; i++) {
		secret_word_copy.push_back(secret_word.at(i));
	}
	
	
	# FIXME!!!!
	// Logic for updating & outputting colors
	for (unsigned int i = 0; i < WORD_LENGTH; i++) {
		char currChar = guess.at(i);
		// If right letter in right position -> green
		if (secret_word.at(i) == currChar) {
			cout << "\x1B[32m" << currChar << " ";
			alphabet[currChar - 65].update_state(STATE_GREEN);
			secret_word_copy.erase(i, 1);
		}
		else {
			bool char_found = false;
			size_t a = 0;
			// If right letter in wrong position -> yellow
			while (!char_found && a < secret_word_copy.length()) {
				if (currChar == secret_word_copy.at(a)) {
					cout << "\x1B[33m" << currChar << " ";
					alphabet[currChar - 65].update_state(STATE_YELLOW);
					bool char_found = true;
					cout << secret_word_copy << endl;
					secret_word_copy.erase(a, 1);
					cout << secret_word_copy << endl;
				}
				a++;
			}	
			// Otherwise white;
			if (!char_found) {
				cout << currChar << " ";
			}
		}
	}
	cout << endl;
}

bool Word::win_state() {
	return false;
}

void Word::print() {
	
}

void Word::print_debugging() {
	cout << secret_word << endl;
	cout << endl;
}


int main() {
	Word test("wordle_answers.dat", "wordle_guesses.dat");
	test.print_debugging();
	test.take_guess("appel");
	return 0;
}