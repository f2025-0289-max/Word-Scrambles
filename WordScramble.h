#ifndef WORDSCRAMBLE_H
#define WORDSCRAMBLE_H

#include "Game.h"
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <random>

using namespace std;

struct WordEntry
{
    string word;
    string hint;
};

class WordScramble : public Game
{
private:
    vector<WordEntry> wordList;
    int lives;
    int totalRounds;
    int roundsWon;

    void loadWords(const string &filename)
    {
        ifstream file(filename);

        if (file.is_open())
        {
            string word, hint;

            while (getline(file, word) && getline(file, hint))
            {
                if (!word.empty() && !hint.empty())
                {
                    wordList.push_back({word, hint});
                }
            }

            file.close();
        }

        // Default words if file not found
        if (wordList.empty())
        {
            wordList =
                {
                    {"programming", "What you do to make software"},
                    {"keyboard", "You type on this"},
                    {"compiler", "Turns code into an executable"},
                    {"variable", "Stores a value in memory"},
                    {"function", "A reusable block of code"},
                    {"algorithm", "Step-by-step problem solution"},
                    {"pointer", "Stores a memory address in C++"},
                    {"inheritance", "OOP concept: child extends parent"},
                    {"polymorphism", "Many forms of one interface"},
                    {"encapsulation", "Bundling data with its methods"}};
        }
    }

    string scramble(const string &word)
    {
        string scrambled = word;

        for (int i = (int)scrambled.size() - 1; i > 0; i--)
        {
            int j = rand() % (i + 1);
            swap(scrambled[i], scrambled[j]);
        }

        // Ensure word actually changes
        if (scrambled == word && word.size() > 1)
        {
            swap(scrambled[0], scrambled[1]);
        }

        return scrambled;
    }

    string toLower(string s)
    {
        for (char &c : s)
        {
            c = tolower(c);
        }

        return s;
    }

    void displayLives()
    {
        cout << "Lives: ";

        for (int i = 0; i < lives; i++)
        {
            cout << "♥ ";
        }

        for (int i = lives; i < 3; i++)
        {
            cout << "♡ ";
        }

        cout << endl;
    }

    bool playRound(const WordEntry &entry)
    {
        lives = 3;

        string scrambled = scramble(entry.word);
        bool hintUsed = false;

        cout << "\nScrambled Word: " << scrambled << endl;
        cout << "Letters: " << entry.word.size() << endl;

        while (lives > 0)
        {
            displayLives();

            cout << "Your guess (or type 'hint' / 'skip'): ";

            string input;
            cin >> input;

            if (toLower(input) == "hint")
            {
                if (!hintUsed)
                {
                    cout << "Hint: " << entry.hint << endl;

                    hintUsed = true;
                    lives--;

                    if (lives == 0)
                    {
                        cout << "Out of lives after using hint!\n";
                        break;
                    }
                }
                else
                {
                    cout << "Hint already used!\n";
                }

                continue;
            }

            if (toLower(input) == "skip")
            {
                cout << "Skipped! The word was: "
                     << entry.word << endl;

                return false;
            }

            if (toLower(input) == toLower(entry.word))
            {
                int points = lives * 10 + (hintUsed ? 0 : 5);

                score += points;

                cout << "Correct! +" << points
                     << " points\n";

                return true;
            }
            else
            {
                lives--;

                if (lives > 0)
                {
                    cout << "Wrong! Try again.\n";
                }
            }
        }

        cout << "The word was: "
             << entry.word << endl;

        return false;
    }

public:
    WordScramble()
        : Game("Word Scramble"),
          lives(3),
          totalRounds(0),
          roundsWon(0)
    {
        srand((unsigned)time(nullptr));
        loadWords("words.txt");
    }

    void showRules() override
    {
        cout << "\n========== WORD SCRAMBLE RULES ==========\n";
        cout << "1. A scrambled word will be shown.\n";
        cout << "2. Guess the correct word.\n";
        cout << "3. You have 3 lives per round.\n";
        cout << "4. Type 'hint' for a clue (-1 life).\n";
        cout << "5. Type 'skip' to skip the word.\n";
        cout << "6. More lives = more points.\n";
        cout << "=========================================\n";
    }

    void play() override
    {
        showRules();

        resetGame();

        char choice;

        cout << "\nReady to play? (y/n): ";
        cin >> choice;

        if (tolower(choice) != 'y')
        {
            return;
        }

        shuffle(
            wordList.begin(),
            wordList.end(),
            default_random_engine((unsigned)time(nullptr)));

        int maxRounds = min((int)wordList.size(), 5);

        for (int i = 0; i < maxRounds; i++)
        {
            cout << "\n========== ROUND "
                 << (i + 1)
                 << " OF "
                 << maxRounds
                 << " ==========\n";

            totalRounds++;

            if (playRound(wordList[i]))
            {
                roundsWon++;
            }

            cout << "Current Score: "
                 << score << endl;

            if (i < maxRounds - 1)
            {
                cout << "\nContinue to next word? (y/n): ";
                cin >> choice;

                if (tolower(choice) != 'y')
                {
                    break;
                }
            }
        }

        cout << "\n========== GAME OVER ==========\n";
        cout << "Rounds Won : "
             << roundsWon
             << " / "
             << totalRounds
             << endl;

        cout << "Final Score: "
             << score
             << endl;
    }

    void resetGame() override
    {
        Game::resetGame();

        lives = 3;
        totalRounds = 0;
        roundsWon = 0;
    }
};

#endif          