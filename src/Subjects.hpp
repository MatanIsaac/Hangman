#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <cstdlib>
#include <random>

namespace Subject
{
    enum struct Subjects
    {
        NONE = 0,
        FOOD,
        COUNTRIES
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
            case Subjects::COUNTRIES: 
                *ret = "COUNTRIES"; 
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
        auto path = "Assets/WordBanks/" + *subjectName + ".txt";
        newFile.open(path);
        if (!newFile.is_open()) 
        {
            printf("Error: Could not open file: %s\n", path.c_str());
            return;
        }
        std::string tmp;
        std::string word;
        
        while (std::getline(newFile, tmp))
        {
            for(auto& c : tmp)
            {
                if(c == ',')
                {
                    subjectVec.push_back(word);
                    word.clear();
                    continue;
                }
                word += c;
            }
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

        std::random_device rd; 
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distr(0, wordBank.size() - 1);

        int randomIndex = distr(gen);
        return wordBank[randomIndex];
    }
};




