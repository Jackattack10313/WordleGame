# Wordle Game
Rudimentary cpp Wordle game that includes the option for the computer to solve for all the possible words in the game.

The solver first prunes the number of words in consideration by looking at the known information. If a green's position is known, it MUST appear in the same position in the words under consideration.
All known yellows must be present in the words under consideration. They must also be placed in different locations that where they originally appeared. A word that includes a grey
letter is immediately pruned. Afterwards, the letters are weighed by letter frequency in the English language (https://www3.nd.edu/~busiforc/handouts/cryptography/letterfrequencies.html). The score it only applied once for each letter (so that words with
many repeating frequent letters are not the only ones chosen).

How to run:
1. Complie with GNU (I use the flags: -std=c++17 -Wall -Wextra -g -pedantic -Weffc++ -fsanitize=undefined,address)
2. Run Driver.cpp
