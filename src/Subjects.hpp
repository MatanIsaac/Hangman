#pragma once

#include <vector>
#include <string>

namespace isaac_hangman
{
    enum class Subjects
    {
        NONE = 0,
        FOOD,
        COUNTRIES
    };

    /// Converts a Subjects enum to its corresponding string representation.
    std::string SubjectToString(Subjects s);

    /// Reads the word bank for the given subject and returns it as a vector of strings.
    std::vector<std::string> GetWordBank(Subjects subject);

    /// Retrieves a random word from the word bank for a given subject.
    std::string GetRandomWord(Subjects subject);
}
