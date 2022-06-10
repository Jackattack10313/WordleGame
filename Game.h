#ifndef GAME_H
#define GAME_H
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <math.h>
#include <chrono>

class Game {
    private:
        std::vector<std::string> words;
        std::string secretWord;

        // Enables computer to narrow down guess in makeGuess()
        std::vector<double> frequencies;
        std::string strTemplate;
        std::vector<std::string> wordsRemaining;
        std::string yellows;
        std::vector<std::string> yellowPositionData;
        std::string greys;
    public:
        Game(std::string wordsFile, std::string frequenciesFile);

        bool validateGuess(std::string guess);
        std::string returnColoredGuess(std::string guess);

        void selectWord();
        void runGameHuman();

        void runGameComputer();
        std::string makeGuess();
};

int find(std::string word, char letter);
bool contains(std::string word, char letter);
#endif