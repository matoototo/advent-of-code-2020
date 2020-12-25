#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

struct Adapter {
    int joltage;
    long long value = -1;
    std::vector<Adapter*> next;
    Adapter(int j): joltage(j) {};
    long long count_children(const int& end) {
        if (this->value != -1) return value;
        long long i = 0;
        for (auto child : next) i += child->count_children(end);
        if (this->joltage == end) return 1;
        return this->value = i;
    }
};

std::vector<int> parseFile(std::ifstream& file) {
    std::vector<int> joltages;
    int i;
    while (file >> i) joltages.push_back(i);
    return joltages;
}

int part_one(const std::vector<int>& joltages) {
    int one = 0, three = 0, last = 0;
    for (int joltage : joltages) {
        if (joltage-last == 1) ++one;
        else if (joltage-last == 3) ++three;
        last = joltage;
    }
    return one*three;
}

long long count_arrangements(const std::vector<int>& joltages) {
    std::vector<Adapter*> adapters;
    Adapter *m1 = new Adapter(-10), *m2 = new Adapter(-10), *m3 = new Adapter(-10);
    for (auto rit = joltages.rbegin(); rit != joltages.rend(); ++rit) { // construct graph
        Adapter* adapter = new Adapter(*rit);
        adapters.push_back(adapter);
        if (m1->joltage - adapter->joltage <= 3) adapter->next.push_back(m1);
        if (m2->joltage - adapter->joltage <= 3) adapter->next.push_back(m2);
        if (m3->joltage - adapter->joltage <= 3) adapter->next.push_back(m3);
        m3 = m2;
        m2 = m1;
        m1 = adapter;
    }
    return adapters.back()->count_children(adapters[0]->joltage);
}

int main() {
    std::ifstream file{"input"};
    auto joltages = parseFile(file);
    joltages.push_back(0); // socket
    std::sort(joltages.begin(), joltages.end());
    joltages.push_back(joltages.back()+3); // built in adapter is always +3 max
    std::cout << part_one(joltages) << std::endl;
    std::cout << count_arrangements(joltages) << std::endl;
    return 0;
}