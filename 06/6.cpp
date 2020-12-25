#include <iostream>
#include <fstream>
#include <algorithm>
#include <set>
#include <string>
#include <utility>

std::pair<std::string, int> next_group(std::ifstream& file) {
    std::string group, s;
    int n = 0;
    while (getline(file, s)) {
        if (s.empty()) break;
        group += s;
        ++n;
    }
    return {group, n};
}

int count_unique(std::ifstream& file) { // part 1
    std::string group;
    int unique = 0;
    while ((group = next_group(file).first) != "") {
        std::set<char> tmp{group.begin(), group.end()};
        unique += tmp.size();
    }
    return unique;
}

int count_every(std::ifstream& file) { // part 2
    std::pair<std::string, int> pair;
    int i = 0;
    while ((pair = next_group(file)).first != "") {
        std::set<char> tmp{pair.first.begin(), pair.first.end()};
        for (char c : tmp) if (std::count(pair.first.begin(), pair.first.end(), c) == pair.second) ++i;
    }
    return i;
}

int main() {
    std::ifstream file{"input"};
    std::cout << count_unique(file) << std::endl;
    file.clear();
    file.seekg(0);
    std::cout << count_every(file) << std::endl;
    return 0;
}
