#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>

std::string next_passport(std::ifstream& file) {
    std::string passport, s;
    while (getline(file, s)) {
        if (s.empty()) break;
        passport += s + " ";
    }
    return passport;
}

bool data_valid(const std::string& data) {
    std::vector<std::string> split;
    std::string s;
    std::istringstream ss(data);
    while (getline(ss, s, ' ')) split.push_back(s);
    for (auto atr : split) {
        if (atr.substr(0, 3) == "byr") {
            int year = stoi(atr.substr(4));
            if (year < 1920 || year > 2002) return false;
        }
        else if (atr.substr(0, 3) == "iyr") {
            int year = stoi(atr.substr(4));
            if (year < 2010 || year > 2020) return false;
        }
        else if (atr.substr(0, 3) == "eyr") {
            int year = stoi(atr.substr(4));
            if (year < 2020 || year > 2030) return false;
        }
        else if (atr.substr(0, 3) == "hgt") {
            if (atr.substr(atr.length()-2) == "cm") {
                int height = stoi(atr.substr(4, atr.length()-6));
                if (height < 150 || height > 193) return false;
            }
            else if (atr.substr(atr.length()-2) == "in") {
                int height = stoi(atr.substr(4, atr.length()-6));
                if (height < 59 || height > 76) return false;
            }
            else return false;
        }
        else if (atr.substr(0, 3) == "hcl") {
            std::string colour = atr.substr(5);
            if (colour.length() != 6 || atr[4] != '#') return false;
            for (auto c : colour) if ((c < '0' || c > '9') && (c < 'a' || c > 'f')) return false;
        }
        else if (atr.substr(0, 3) == "ecl") {
            std::string colour = atr.substr(4);
            std::vector<std::string> colours = {"amb", "blu", "brn", "gry", "grn", "hzl", "oth"};
            bool contains = false;
            for (auto c : colours) if (c == colour) contains = true;
            if (!contains) return false;
        }
        else if (atr.substr(0, 3) == "pid") {
            std::string id = atr.substr(4);
            if (id.length() != 9) return false;
            for (auto c : id) if (c < '0' || c > '9') return false;
        }
    }
    return true;
}

int valid_passports(std::ifstream& file, bool second = false) {
    int n = 0;
    std::string s;
    std::vector<std::string> reqs = {"ecl", "pid", "eyr", "hcl", "byr", "iyr", "hgt"};
    while ((s = next_passport(file)) != "") {
        bool valid = true;
        for (auto req : reqs) if (s.find(req) == std::string::npos) valid = false;
        if (second) valid *= data_valid(s);
        if (valid) ++n;
    }
    return n;
}

int main() {
    std::ifstream file("input");
    std::cout << valid_passports(file) << std::endl; // first
    file.clear();
    file.seekg(0); // move ifstream to beginning of file
    std::cout << valid_passports(file, true) << std::endl; // second
    return 0;
}