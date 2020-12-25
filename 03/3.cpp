#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <utility>

int cheap_toboggan(const std::vector<std::string>& slope) { // part 1
    int trees = 0;
    int location = 3;
    for (auto it = slope.begin()+1; it != slope.end(); ++it) {
        if (it->at(location) == '#') ++trees;
        location = (location+3)%it->length();
    }
    return trees;
}

int toboggan(const std::vector<std::string>& slope, const int& right, const int& down) { // part 2
    int trees = 0;
    int location = right;
    for (auto it = slope.begin()+down; it < slope.end(); it += down) {
        if (it->at(location) == '#') ++trees;
        location = (location+right)%it->length();
    }
    return trees;
}

int main() {
    std::ifstream file("input");
    std::vector<std::string> input;
    std::string s;
    while (file >> s) input.push_back(s);
    std::cout << cheap_toboggan(input) << std::endl; // part 1
    std::vector<std::pair<int, int>> config = {{1, 1}, {3, 1}, {5, 1}, {7, 1}, {1, 2}};
    int product = 1;
    for (auto pair : config) product *= toboggan(input, pair.first, pair.second);
    std::cout << product << std::endl; // part 2
    return 0;
}