#ifndef TRANSPOSITION_H
#define TRANSPOSITION_H
#include <unordered_map>
#include <sstream>

class TranspositionTable {
private:
    std::unordered_map<std::string, std::pair<int, int>> table;

    std::vector<std::string> splitString(const std::string& input, char delimiter) {
        std::vector<std::string> tokens;
        std::istringstream iss(input);
        std::string token;

        while (std::getline(iss, token, delimiter)) {
            tokens.push_back(token);
        }

        return tokens;
    }

    std::string joinStrings(const std::vector<std::string>& tokens, char delimiter) {
        std::ostringstream oss;

        if (!tokens.empty()) {
            oss << tokens[0];  // Add the first token

            // Add the remaining tokens with the delimiter
            for (size_t i = 1; i < tokens.size(); ++i) {
                oss << delimiter << tokens[i];
            }
        }

        return oss.str();
    }

public:
    void store(const std::string& key, int depth, int value) {
        std::vector<std::string> tokens = splitString(key, ' ');
        tokens.pop_back(); tokens.pop_back();
        std::string joinedString = joinStrings(tokens, ' ');
        table[joinedString] = std::make_pair(depth, value);
    }

    std::pair<int, int> lookup(const std::string& key) {
        std::vector<std::string> tokens = splitString(key, ' ');
        tokens.pop_back(); tokens.pop_back();
        std::string joinedString = joinStrings(tokens, ' ');
        auto it = table.find(joinedString);
        if (it != table.end()) {
            return (it->second);
        } else {
            return {-1,-1};
        }
    }

    void print() {
        for (const auto xx: table) {
            std::cout << xx.first << "->" << xx.second.first << " " << xx.second.second << std::endl;
        }
    }

    void size() {
        std::cout << table.size() << std::endl;
    }
};


#endif