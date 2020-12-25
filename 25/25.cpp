#include <fstream>
#include <iostream>

long long transform(long long loop_size, long long subject, long long value = 1) {
    for (int i = 0; i < loop_size; ++i) {
        value *= subject;
        if (value > 20201227) value %= 20201227;
    }
    return value;
}

long long find_loop_size(long long key) {
    long long value = 1, loop_size = 1;
    for (loop_size = 1; value != key; ++loop_size) {
        value = transform(1, 7, value);
    }
    return --loop_size;
}

long long p1(std::ifstream& file) {
    long long card_public, door_public;
    file >> card_public;
    file >> door_public;
    long long card_loop = find_loop_size(card_public);
    long long door_loop = find_loop_size(door_public);
    long long card_priv = transform(door_loop, card_public);
    return card_priv;
}

int main() {
    std::ifstream file{"/home/matoototo/Programming/aoc/25/input"};
    std::cout << p1(file) << std::endl;
    return 0;
}