// Updated 25 August 2023

#include <string>
#include <algorithm>
#include <sstream>
#include <iterator>
#include "StringHelpers.hpp"

void strHelp::strip(std::string& s) {
    s.erase(remove_if(s.begin(), s.end(), isspace), s.end());
}

void strHelp::format(std::string& s) {
    std::transform(s.begin(), s.end(), s.begin(), [&](char c) -> char {return std::tolower(c);});
    strHelp::strip(s);
}

bool strHelp::isVowel(unsigned char c) {
    // method for checking taken from 
    // https://stackoverflow.com/questions/47846406/c-fastest-way-to-check-if-char-is-vowel-or-consonant
    return (0x208222 >> (c & 0x1f)) & 1;
}

void strHelp::lower(std::string& s) {
    std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c){return std::tolower(c);});
}

std::vector<std::string> strHelp::split(const std::string& str) {
    std::istringstream iss{str};
    return std::vector<std::string>{std::istream_iterator<std::string>{iss}, std::istream_iterator<std::string>{}};
}

std::vector<std::string> strHelp::reduce(const std::vector<std::string>& v, const std::vector<std::string>& words) {
    std::vector<std::string> out;
    std::for_each(v.begin(), v.end(), [&](const std::string& s){
        if (s.size() > 0 && std::find(words.begin(), words.end(), s) == words.end()) out.push_back(s);
    });
    return out;
}

// adapted from https://stackoverflow.com/questions/4654636/how-to-determine-if-a-string-is-a-number-with-c
bool strHelp::isNumber(const std::string& s) {
    return !s.empty() && std::find_if(s.begin(), s.end(), [](unsigned char c){return !std::isdigit(c) && c != '.';}) == s.end();
}

std::string strHelp::join(std::vector<std::string> strings, std::string delim) {
    if (strings.size() < 1) return "";
    else if (strings.size() == 1) return strings[0];
    else {
        std::stringstream ss;
        std::vector<std::string>::iterator end = strings.end() - 1;
        std::for_each(strings.begin(), end, [&](const std::string& s){ss << s << delim << " ";});
        ss << *end;
        return ss.str();
    }
}