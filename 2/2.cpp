#include <iostream>
#include <fstream>
#include <sstream>
#include <utility>

bool is_valid(const int& min, const int& max, const char& c, const std::string& password) {
    int cur = 0;
    for (char l : password) if (l == c) ++cur;
    return max >= cur && min <= cur;
}

bool is_valid2(const int& first, const int& second, const char& c, const std::string& password) {
    if (password[first] == c && password[second] == c) return false;
    if (password[first] == c || password[second] == c) return true;
    return false;
}

std::pair<int, int> validate(std::ifstream& file) {
    int valid = 0;
    int valid2 = 0;
    std::string s;
    while (getline(file, s)) {
        std::istringstream ss(s);
        getline(ss, s, '-');
        int min = stoi(s);
        getline(ss, s, ' ');
        int max = stoi(s);
        getline(ss, s, ':');
        char c = s.c_str()[0];
        ss.ignore(1);
        getline(ss, s);
        valid += is_valid(min, max, c, s);
        valid2 += is_valid2(--min, --max, c, s);
    };
    return std::pair<int, int>(valid, valid2);
}

int main() {
    std::ifstream file("input");
    std::pair<int, int> result = validate(file);
    std::cout << result.first << " " << result.second << std::endl;
    return 0;
}