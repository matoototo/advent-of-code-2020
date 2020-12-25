#include <iostream>
#include <fstream>
#include <vector>

std::vector<long long> parse_file(std::fstream& file) {
    std::vector<long long> input;
    long long i;
    while (file >> i) input.push_back(i);
    return input;
}

long long find_faulty(const std::vector<long long>& input) {
    for (auto i = 25; i != input.size(); ++i) {
        bool good = false;
        for (auto j = 25-i; j != i; ++j) {
            for (auto k = j; k != i; ++k) {
                good += (input[j]+input[k] == input[i]);
            }
        }
        if (!good) return input[i];
    }
    return -1;
}

long long find_weakness(const std::vector<long long>& input, const long long& faulty) {
    for (auto i = 0; i != input.size(); ++i) {
        long long curr = input[i], min = input[i], max = input[i];
        for (auto j = i+1; j != input.size(); ++j) {
            curr += input[j];
            if (input[j] > max) max = input[j];
            else if (input[j] < min) min = input[j]; 
            if (curr == faulty) return min+max;
        }
    }
    return -1;
}

int main() {
    std::fstream file{"input"};
    auto input = parse_file(file);
    auto faulty = find_faulty(input);
    std::cout << faulty << std::endl;
    std::cout << find_weakness(input, faulty) << std::endl;
    return 0;
}