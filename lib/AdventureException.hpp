// updated 13 February 2022

#ifndef ADVENTUREEXCEPTION_HPP
#define ADVENTUREEXCEPTION_HPP

#include <string>

class AdventureException {
    public:
        AdventureException(const std::string& reason);
        std::string getReason() const noexcept;

    private:
        std::string reason;
};

inline AdventureException::AdventureException(const std::string& reason) : reason{reason} {
}

inline std::string AdventureException::getReason() const noexcept {
    return reason;
}

#endif