#include "game.h"

using std::cout;
using std::endl;
using std::vector;
using std::string;

Game::Game(string wordsFile, string frequenciesFile) : words(0), secretWord(""), frequencies(0), strTemplate(""), 
wordsRemaining(0), yellows(""), yellowPositionData(0), greys("") {
    std::fstream input1(wordsFile);

    while(!input1.eof()) {
        string temp;
        input1 >> temp;
        words.push_back(temp);
    }

    // Wrote a quick insertion sort so I can use a binary search to minimize input validation time
    // Consider writing a merge sort!
    unsigned int i = 1;
    unsigned int j;
    while (i < words.size()) {
        j = i;
        while (j > 0 && words.at(i) < words.at(j - 1)) {
            j--;
        }
        string temp = words.at(i);
        words.erase(words.begin() + i);
        words.insert(words.begin() + j, temp);
        i++;
    }

    // Inserting the frequency percentage per letter in order (0 = a -> 25 = z)
    std::fstream input2(frequenciesFile);
    while (!input2.eof()) {
        char letter;
        double frequency;
        input2 >> letter >> frequency;
        frequencies.push_back(frequency);
    }
}

// Chooses the secret word (seeded with the current time)
void Game::selectWord() {
    std::srand(time(0));
    int choice = static_cast<double>(std::rand()) / INT32_MAX * words.size();
    secretWord = words.at(choice);
}

// Verifies that a guess is valid (human input)
bool Game::validateGuess(string guess) {
    // All guesses must be alphabetic and 5 characters in length
    if (guess.length() == 5) {
        for (unsigned int i = 0; i < guess.length(); i++) {
            if (!isalpha(guess.at(i))) {
                return false;
            }
            // Converting to lowercase
            guess.at(i) = tolower(guess.at(i));
        }
        // Binary search
        unsigned int lowerBound = 0;
        unsigned int upperBound = words.size() - 1;
        while (lowerBound <= upperBound) {
            unsigned int pos = (upperBound + lowerBound) / 2;       
            if (guess == words.at(pos)) {
                return true;
            }
            else if (guess > words.at(pos)) {
                lowerBound = pos + 1;
            }
            else if (guess < words.at(pos)) {
                upperBound = pos - 1;
            }
        }
    }
    return false;
}

string Game::returnColoredGuess(string guess) {
    string colors = "";
    string copySecret = secretWord.substr();
    for (unsigned int i = 0; i < guess.length(); i++) {
        if (copySecret.at(i) == guess.at(i)) {
            // Outputting a green background letter and adding g (green) to string
            cout << "\033[42m" + guess.substr(i, 1) + "\033[0m";
            colors.append("g");
            copySecret.at(i) = '.';
        }
        else {
            bool found = false;
            unsigned int a = 0;
            while(a < copySecret.length() && !found) {
                if (copySecret.at(a) == guess.at(i)) {
                    // Outputting a yellow background letter and adding y (yellow) to string
                    cout << "\033[43m" + guess.substr(i, 1) + "\033[0m";
                    colors.append("y");
                    copySecret.at(a) = '.';
                    found = true;
                }
                a++;
            }
            if (!found) {
                // Outputting a grey background letter and adding w (grey/white - g already in use) to string
                cout << "\033[100m" + guess.substr(i, 1) + "\033[0m";
                colors.append("w");
            }
        }
    }
    cout << endl;
    return colors;
}

// Version that uses input validation and cin
void Game::runGameHuman() {
    // Continue boolean & stats variables
    bool continueGame;
    unsigned int gamesPlayed = 0;
    unsigned int gamesWon = 0;
    unsigned int totalScore = 0;
    // Do while loop!
    do {
        gamesPlayed++;
        cout << "Game: " << gamesPlayed << endl;

        // Running game
        selectWord();
        unsigned int turns = 0;
        bool gameOver = false;
        while (turns < 6 && !gameOver) {
            string guess;
            do {
                cout << "Make a guess: ";
                std::cin >> guess;
            }
            while (!validateGuess(guess));
            returnColoredGuess(guess);
            if (guess == secretWord) {
                gameOver = true;
            }
            turns++;
        }

        // Checking win condition (true -> won)
        if (gameOver) {
            cout << "Won in " << turns << " turns." << endl;
            gamesWon++;
            totalScore += turns;
        }
        else {
            cout << "You lost. The word was " << secretWord << endl;
        }

        // Checking to see if game should continue
        cout << endl;
        cout << "Continue? [Y/n]: ";
        string input;
        std::cin >> input;
        // Only accepts Y/y otherwise counts as no
        if (input.length() == 1 && tolower(input.at(0)) == 'y') {
            continueGame = true;
        }
        else {
            // Printing stats about game
            printf("Won %d games (%.2f%%).\n", gamesWon, static_cast<double>(gamesWon) / gamesPlayed * 100);
            printf("Average score of %.2f for games won.\n", static_cast<double>(totalScore) / gamesWon);
            continueGame = false;
        }
    } while (continueGame);
}

// Version that does not use input validation (runs for every possible word)
void Game::runGameComputer() {
    // Stats variables
    auto start = std::chrono::high_resolution_clock::now();
    unsigned int gamesPlayed = 0;
    unsigned int gamesWon = 0;
    unsigned int totalScore = 0;

    // Running through entire word list
    for (unsigned int i = 0; i < words.size(); i++) {
        gamesPlayed++;
        secretWord = words.at(i);

        // Outputting information so user can see what computer is doing
        cout << "Game: " << gamesPlayed << endl;
        cout << "Secret word: " << secretWord << endl;

        // Resetting helper variables
        strTemplate = ".....";
        for (unsigned int a = 0; a < words.size(); a++) {
            wordsRemaining.push_back(words.at(a));
        }
        yellows = "";
        yellowPositionData.clear();
        for (unsigned int a = 0; a < 6; a++) {
            yellowPositionData.push_back("");
        }
        greys = "";

        // Running game
        unsigned int turns = 0;
        bool gameOver = false;
        while (turns < 6 && !gameOver) {
            // Making a guess and getting feedback from the game
            string guess = makeGuess();
            string feedback = returnColoredGuess(guess);            

            for (unsigned int letter = 0; letter < feedback.size(); letter++) {
                char guessLetter = guess.at(letter);
                if (feedback.at(letter) == 'g' && strTemplate.at(letter) == '.') {
                    if (contains(yellows, guessLetter)) {
                        yellows.erase(find(yellows, guessLetter), 1);
                    }
                    strTemplate.at(letter) = guessLetter;
                }
                else if (feedback.at(letter) == 'y' && !contains(yellows, guessLetter)) {
                    yellows.push_back(guessLetter);
                }
                else if (feedback.at(letter) == 'w' && !contains(greys, guessLetter)) {
                    greys.push_back(guessLetter);
                }

                if (feedback.at(letter) == 'y' && !contains(yellowPositionData.at(letter), guessLetter)) {
                    yellowPositionData.at(letter).push_back(guessLetter);
                }
            }
            // Win condition
            if (guess == secretWord) {
                gameOver = true;
            }
            turns++;
        }
        if (gameOver) {
            cout << "Won in " << turns << " turns." << endl;
            gamesWon++;
            totalScore += turns;
        }
        else {
            cout << "Computer lost." << endl;
        }
    }
    // Game statistics
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(stop - start);
    printf("Won %d games (%.2f%%).\n", gamesWon, static_cast<double>(gamesWon) / gamesPlayed * 100);
    printf("Average score of %.2f for games won.\n", static_cast<double>(totalScore) / gamesWon);
    cout << "Time taken to run " << words.size() << " games: " << duration.count() << " seconds." << endl;
}

// Algorithm reduces the number of words remaining based on known criteria about letters (alpha-beta pruning)
// Then uses letter frequencies to assign a score to the words (each letter will only count once to maximize information gained)
string Game::makeGuess() {
    // Paring down word list
    // First checking to see if any information about the secret word is known
    if (strTemplate != "....." || yellows.length() != 0 || greys.length() != 0) {
        // Iterating through the word list
        for (unsigned int word = 0; word < wordsRemaining.size(); word++) {
            // Variables to help with the 
            bool valid = true;
            string currWord = wordsRemaining.at(word);
            string yellowsCopy = yellows.substr();
            for (unsigned int letter = 0; letter < currWord.length(); letter++) {
                char currLetter = currWord.at(letter);
                // Condition to see if a green letter matches the letter in the current word
                if (strTemplate.at(letter) != '.' && strTemplate.at(letter) != currLetter) {
                    valid = false;
                }
                // Condition to see if a yellow letter appears in a position previously held by the same letter (cannot happen)
                else if (contains(yellowsCopy, currLetter) && contains(yellowPositionData.at(letter), currLetter)) {
                    valid = false;
                }
                // Condition that empties the yellowsCopy list (must eliminate yellows before ruling a word out 
                // - e.g. a word could have ee and the first could be yellow and the second grey)
                else if (contains(yellowsCopy, currLetter)) {
                    yellowsCopy.erase(find(yellowsCopy, currLetter), 1);
                }
                // If it contains a known grey letter it must be invalid - must also check for letter being green - possible for a grey & green letter to exist that are 
                // the same -> improper invalidation
                else if (contains(greys, currLetter) && strTemplate.at(letter) != currLetter) {
                    valid = false;
                }
            }
            // Words must contain all yellows that are known!
            if (yellowsCopy.length() > 0) {
                valid = false;
            }
            // Removing and decreasing the increment so words are not skipped over
            if (!valid) {
                wordsRemaining.erase(wordsRemaining.begin() + word);
                word--;
            }
        }
    }

    // Assigning each word a score - returning the highest score
    double highestScore = 0;
    string highScoreWord = "";
    for (unsigned int i = 0; i < wordsRemaining.size(); i++) {
        double score = 0;
        // Iterating through each letter in the alphabet and attempting to find it in the word
        for (unsigned int alph = 0; alph < 26; alph++) {
            unsigned int letter = 0; 
            bool found = false;
            while (letter < wordsRemaining.at(i).size() && !found) {
                if (wordsRemaining.at(i).at(letter) == static_cast<int>(alph) + 97) {
                    score += frequencies.at(alph);
                    found = true;
                }
                letter++;
            }
        }
        // If score is higher than highestScore, make it the highestScore
        if (score > highestScore) {
            highestScore = score;
            highScoreWord = wordsRemaining.at(i);
        }
    }
    return highScoreWord;
}

int find(string word, char letter) {
    unsigned int i = 0;
    while (i < word.length()) {
        if (word.at(i) == letter) {
            return i;
        }
        i++;
    }
    return -1;
}

bool contains(string word, char letter) {
    if (find(word, letter) != -1) {
        return true;
    }
    return false;
}