#include <iostream>
#include <fstream>
#include <vector>
#include <iterator>
#include <string>

int find_two(std::ifstream& file) {
    std::vector<int> input;
    for (int in; file >> in; ) {
        input.push_back(in);
        for (auto it = input.begin(); it != input.end(); ++it) if (*it+in == 2020) return *it*in;
    };
    return -1;
}

int find_three(std::ifstream& file) {
    std::vector<int> input;
    for (int in; file >> in; ) {
        input.push_back(in);
        for (auto it = input.begin(); it != input.end(); ++it)
            for (auto it2 = input.begin(); it2 != input.end(); ++it2) 
                if (*it+*it2+in == 2020) return (*it)*(*it2)*in;
    }
    return -1;
}


int main () {
    std::ifstream file("input");
    std::cout << find_two(file) << '\n';
    std::cout << find_three(file) << '\n';
    return 0;
}