#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <cstdlib>
#include <random>

struct Subject
{
    enum struct Subjects
    {
        NONE = 0,
        FOOD
    };

    /// @brief  // This function will convert the Subjects enum to a string
    /// @param s the Subjects enum to convert
    static std::string* SubjectToString(Subjects s)
    {
        std::string* ret = new std::string();
        switch (s)
        {
            case Subjects::NONE: 
                *ret = "NONE"; 
                break;
            case Subjects::FOOD: 
                *ret = "FOOD"; 
                break;
            default: 
                *ret = "Out of range";
                break;
        }
        return ret;
    }
   
    /// @brief  // This function will read the word bank from a file and 
    /// store it in the vector passed in as a parameter
    /// @param subjectVec the vector to store the word bank in
    /// @param subject the subject to read the word bank from
    static void GetWordBank(std::vector<std::string>& subjectVec, Subjects subject)
    {
        if( subject == Subjects::NONE ) return;
        std::string* subjectName = SubjectToString(subject);
        std::ifstream newFile;
        auto path = "Assets\\WordBanks\\" + *subjectName + ".txt";
        newFile.open(path);
        if (!newFile.is_open()) 
        {
            printf("Error: Could not open file: %s\n", path.c_str());
            return;
        }
        std::string word;
        
        while (newFile >> word)
        {
            subjectVec.push_back(word);
        }
    }

    static std::string GetRandomWord(Subjects subject)
    {
        if (subject == Subjects::NONE) return "NONE";
        std::vector<std::string> wordBank;
        GetWordBank(wordBank, subject);

        if (wordBank.empty())
        {
            printf("Error: Word bank is empty\n");
            return "NONE";
        }

        std::random_device rd; // obtain a random number from hardware
        std::mt19937 gen(rd()); // seed the generator
        std::uniform_int_distribution<> distr(0, (int)wordBank.size()); // range is from 0 to wordBank.size() - 1

        int randomIndex = distr(gen);
        printf("Random Index: %d - ", randomIndex);
        printf("Random Word: %s\n", wordBank[randomIndex].c_str());
        return wordBank[randomIndex];
    }
};




