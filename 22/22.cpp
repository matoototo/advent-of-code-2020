#include <iostream>
#include <fstream>
#include <deque>
#include <unordered_set>

struct Game {
    std::unordered_set<std::string> previous;
    std::deque<int> p1, p2;
    Game(std::deque<int> p1_init, std::deque<int> p2_init): p1(p1_init), p2(p2_init) {}
    bool play() {
        bool p1_wins = false;
        if (p1.empty() || p2.empty()) return p2.empty();
        if (!add_previous()) return true;
        if (p1[0] < p1.size() && p2[0] < p2.size()) {
            std::deque<int> p1_cpy, p2_cpy;
            for (int i = 1; i <= p1[0]; ++i) p1_cpy.push_back(p1[i]);
            for (int i = 1; i <= p2[0]; ++i) p2_cpy.push_back(p2[i]);
            Game sub{p1_cpy, p2_cpy};
            p1_wins = sub.play();
        } else {
            p1_wins = p1[0] > p2[0];
        }
        if (p1_wins) {
            p1.push_back(p1[0]);
            p1.push_back(p2[0]);
        } else {
            p2.push_back(p2[0]);
            p2.push_back(p1[0]);
        }
        p1.pop_front();
        p2.pop_front();
        return play();
    }
    bool add_previous() {
        std::string s{p1.begin(), p1.end()};
        s += " ";
        std::string s2{p2.begin(), p2.end()};
        s += s2;
        return previous.insert(s).second;
    }

};

std::deque<int> parse_deck(std::ifstream& file) {
    std::deque<int> deck;
    std::string s;
    while (getline(file, s) && s != "") {
        if (s[0] == 'P') continue;
        deck.push_back(std::stoi(s));
    }
    return deck;
}

void play(std::deque<int>& p1, std::deque<int>& p2) {
    if (p1.empty() || p2.empty()) return;
    if (p1[0] > p2[0]) {
        p1.push_back(p1[0]);
        p1.push_back(p2[0]);
    } else {
        p2.push_back(p2[0]);
        p2.push_back(p1[0]);
    }
    p1.pop_front();
    p2.pop_front();
    play(p1, p2);
}

int part1(std::deque<int> p1, std::deque<int> p2) {
    play(p1, p2);
    std::deque<int> winner = p1;
    if (p1.empty()) winner = p2;
    int score = 0;
    for (int i = 0; i < winner.size(); ++i) {
        score += winner[i]*(winner.size()-i);
    }
    return score;
}

int part2(const std::deque<int>& p1, const std::deque<int>& p2) {
    Game initial{p1, p2};
    initial.play();
    std::deque<int> winner = initial.p1;
    if (winner.empty()) winner = initial.p2;
    int score = 0;
    for (int i = 0; i < winner.size(); ++i) {
        score += winner[i]*(winner.size()-i);
    }
    return score;
}

int main() {
    std::ifstream file{"/home/matoototo/Programming/aoc/22/input"};
    auto p1 = parse_deck(file);
    auto p2 = parse_deck(file);
    std::cout << part1(p1, p2) << std::endl;
    std::cout << part2(p1, p2) << std::endl;
    return 0;
}