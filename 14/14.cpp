#include <unordered_map>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <cmath>
#include <sstream>

using ull = unsigned long long;
using memory = std::unordered_map<ull, ull>;

void part_1(const ull& or_mask, const ull& and_mask, const ull& him_mask, ull& loc, ull& num, memory& mem) {
    num |= or_mask;
    num &= and_mask;
    mem[loc] = num; 
}

void recursive_build(ull current, int i, const std::vector<int>& locations, std::vector<ull>& combs, bool added = false) {
    if (i <= locations.size()) {
        if (added) combs.push_back(current);
        recursive_build(current+(ull)std::pow(2, locations[i]), i+1, locations, combs, true);
        recursive_build(current, i+1, locations, combs);
    }
}

std::vector<ull> combinations(const ull& him_mask) {
    std::vector<ull> combs;
    std::vector<int> locations;
    for (int i = 0; i < 64; ++i) if ((him_mask >> i) & 1) locations.push_back(i);
    recursive_build(0, 0, locations, combs, true);
    return combs;
}

void part_2(const ull& or_mask, const ull& and_mask, const ull& him_mask, ull& loc, ull& num, memory& mem) {
    loc |= or_mask;
    loc &= ~him_mask;
    auto him_masks = combinations(him_mask); // is called too many times (every mem), possible optimization
    for (auto him : him_masks) {
        mem[loc|him] = num;
    }
}

memory parse_file(std::ifstream& file, bool p2 = false) {
    std::string s;
    ull or_mask, and_mask, him_mask;
    memory mem;
    while (getline(file, s)) {
        if (s[1] == 'a') {
            or_mask = 0; and_mask = -1; him_mask = 0;
            for (int i = 7; i < s.size(); ++i) {
                if (s[i] == '1') or_mask += (ull)std::pow(2, 35-i+7);
                else if (s[i] == '0') and_mask -= (ull)std::pow(2, 35-i+7);
                else if (s[i] == 'X') him_mask += (ull)std::pow(2, 35-i+7);
            }
        }
        else {
            std::istringstream ss{s};
            getline(ss, s, '[');
            getline(ss, s, ']');
            ull loc = std::stoull(s);
            getline(ss, s, '=');
            getline(ss, s);
            unsigned long long num = std::stoi(s);
            if (!p2) part_1(or_mask, and_mask, him_mask, loc, num, mem);
            else part_2(or_mask, and_mask, him_mask, loc, num, mem);
        }
    }
    return mem;
}

int main() {
    std::ifstream file{"input"};
    memory mem = parse_file(file, true);
    unsigned long long sum = 0;
    for (auto loc : mem) sum += loc.second;
    std::cout << sum << std::endl;
    return 0;
}