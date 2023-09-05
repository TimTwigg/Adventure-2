// Updated 25 August 2023

#ifndef STRINGHELPERS_HPP
#define STRINGHELPERS_HPP

#include <string>
#include <vector>

namespace strHelp {
    // unimportant words list
    const std::vector<std::string> uWords = {
        "the", "is", "a", "an", "from", "out", "of", "with", "this"
    };

    // remove whitespace
    void strip(std::string& s);
    // remove whitespace and convert to lowercase
    void format(std::string& s);
    // check if char is a vowel
    bool isVowel(unsigned char c);
    // convert to lowercase
    void lower(std::string& s);
    // split a string on whitespace and return the words in a vector
    std::vector<std::string> split(const std::string& str);
    // remove unimportant words
    std::vector<std::string> reduce(const std::vector<std::string>& v, const std::vector<std::string>& words = strHelp::uWords);
    // check if string is a positive number
    bool isNumber(const std::string& s);
    // join a vector of strings into one string, separated by the given delimiter
    std::string join(std::vector<std::string> strings, std::string delim = ",");
}

#endif