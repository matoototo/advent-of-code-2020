#include <fstream>
#include <iostream>
#include <vector>
#include <unordered_map>

using umap = std::unordered_map<int, int>;

std::pair<umap, int> get_starting(std::ifstream& file) {
    std::unordered_map<int, int> starting;
    std::string s;
    int i = 0;
    int num = 0;
    while (getline(file, s, ',')) {
        num = std::stoi(s); 
        starting.insert(std::make_pair(num, ++i));
    }
    starting.erase(num);
    return std::make_pair(starting, num);
}

int nth_spoken(umap spoken, int last, int n) {
    int turn = spoken.size();
    while (turn < n) {
        if (spoken[last] != 0) {
            int tmp = turn-spoken[last]+1;
            spoken[last] = ++turn;
            if (turn == n) return last;
            last = tmp;
        }
        else {
            spoken[last] = ++turn;
            if (turn == n) return last;
            last = 0;
        }
    }
    return -1;
}

int main() {
    std::ifstream file{"/home/matoototo/Programming/aoc/15/input"};
    auto starting = get_starting(file);
    std::cout << nth_spoken(starting.first, starting.second, 30000000) << std::endl;
    return 0;
}