#include "Subjects.hpp"
#include <fstream>
#include <sstream>
#include <random>
#include <stdexcept>
#include <iostream>
#include <unordered_map>

namespace isaac_hangman
{
    /// A compile-time map for enum-to-string conversions.
    std::string SubjectToString(Subjects s)
    {
        static const std::unordered_map<Subjects, std::string> subjectToString{
            {Subjects::NONE, "NONE"},
            {Subjects::FOOD, "FOOD"},
            {Subjects::COUNTRIES, "COUNTRIES"}};

        auto it = subjectToString.find(s);
        return it != subjectToString.end() ? it->second : "UNKNOWN";
    }

    /// Reads a word bank file into a vector of strings.
    std::vector<std::string> GetWordBank(Subjects subject)
    {
        if (subject == Subjects::NONE)
            return {};

        const std::string subjectName = SubjectToString(subject);
        const std::string filePath = "Assets/WordBanks/" + subjectName + ".txt";

        std::ifstream file(filePath);
        if (!file)
        {
            throw std::runtime_error("Error: Could not open file: " + filePath);
        }

        std::vector<std::string> words;
        std::string line;

        while (std::getline(file, line))
        {
            std::istringstream lineStream(line);
            std::string word;
            while (std::getline(lineStream, word, ','))
            {
                words.emplace_back(std::move(word));
            }
        }

        return words;
    }

    /// Selects a random word from the given subject's word bank.
    std::string GetRandomWord(Subjects subject)
    {
        if (subject == Subjects::NONE)
        {
            return SubjectToString(Subjects::NONE);
        }

        std::vector<std::string> wordBank;
        try
        {
            wordBank = GetWordBank(subject);
        }
        catch (const std::exception& e)
        {
            std::cerr << e.what() << '\n';
            return "ERROR";
        }

        if (wordBank.empty())
        {
            std::cerr << "Error: Word bank is empty for subject: " << SubjectToString(subject) << '\n';
            return "NONE";
        }

        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, static_cast<int>(wordBank.size() - 1));

        return wordBank[dis(gen)];
    }
}
