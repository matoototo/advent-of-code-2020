#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include <cmath>
#include <algorithm>

std::vector<int> parseFile(std::ifstream& file) {
    std::vector<int> ids;
    std::string s;
    while (file >> s) {
        int row = 0, column = 0;
        for (auto i = 0; i < 7; ++i) if (s[i] == 'B') row += pow(2, 6-i);
        for (auto i = 0; i < 3; ++i) if (s[i+7] == 'R') column += pow(2, 2-i);
        ids.push_back(row*8+column);
    }
    return ids;
}

int findMaxID(const std::vector<int>& ids) {
    int max = 0;
    for (auto id : ids) max = std::max(max, id);
    return max;
}

int findSeatID(std::vector<int>& ids) {
    std::sort(ids.begin(), ids.end());
    int prev = ids[0]-1;
    for (auto id : ids) {
        if (id != prev+1) return id-1;
        prev = id;
    }
    return -1;
}

int main() {
    std::ifstream file("input");
    auto ids = parseFile(file);
    std::cout << findMaxID(ids) << std::endl;
    std::cout << findSeatID(ids) << std::endl;
    return 0;
}