# Wordle Game
Rudimentary cpp Wordle game that includes the option for the computer to solve for all the possible words in the game.

The solver first prunes the number of words in consideration by looking at the known information. If a green is known, it MUST appear in the words under consideration.
All yellows must be present in the words under consideration. They must also be placed in different locations that where they originally appeared. A word that includes a grey
letter is immediately pruned. Afterwards, the letters are weighed by letter frequency in the English language. The score it only applied once for each letter (so that words with
many repeating frequent letters are not the only ones chosen. Increasing the number of letters discovered yields more information about the word).

How to run:
1. Complie with GNU (I use g++ -std=c++17 -Wall -Wextra -g -pedantic -Weffc++ -fsanitize=undefined,address \*.cpp)
2. Run Driver.cpp
