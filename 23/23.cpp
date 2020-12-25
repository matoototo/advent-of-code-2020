#include <iostream>
#include <fstream>
#include <deque>
#include <unordered_set>
#include <vector>
#include <string>
#include <algorithm>

struct Node {
    int val;
    Node* next;
    Node(int x): val(x) {}
};

void simulate(std::deque<int>& cups, std::deque<int>::iterator& curr) {
    std::deque<int> next_three, copy = cups;
    int curr_num = *curr;
    
    auto it = curr+1;
    for (int i = 0; i < 3; ++i) {
        if (it == cups.end()) it = cups.begin();
        next_three.push_back(*it);
        copy.erase(std::find(copy.begin(), copy.end(), next_three.back()));
        ++it;
    }
    
    int dest_num = *curr-1;
    auto dest_loc = copy.end();
    while (dest_loc == copy.end()) {
        if (dest_num < 1) dest_num = 9;
        dest_loc = std::find(copy.begin(), copy.end(), dest_num);
        --dest_num;
    }
    ++dest_num;
    copy.insert(++dest_loc, next_three.begin(), next_three.end());
    cups = copy;
    curr = std::find(cups.begin(), cups.end(), curr_num)+1;
    if (curr == cups.end()) curr = cups.begin();
}

std::deque<int> get_cups(std::ifstream& file) {
    std::string s;
    file >> s;
    std::deque<int> cups;
    for (auto c : s) cups.push_back(c-'0');
    return cups;
}

void p1(std::deque<int> cups) {
    auto it = cups.begin();
    for (int i = 0; i < 100; ++i) simulate(cups, it);
    for (auto it = ++std::find(cups.begin(), cups.end(), 1); it != cups.end(); ++it) std::cout << *it;
    for (auto it = cups.begin(); it != std::find(cups.begin(), cups.end(), 1); ++it) std::cout << *it;
    std::cout << std::endl;
}

void p2(std::deque<int>& cups) {
    std::vector<Node> nodes;
    for (auto i = 0; i < 1e6; ++i) nodes.emplace_back(i+1);
    for (auto i = 0; i < 1e6-1; ++i) nodes[i].next = &nodes[i+1];
    nodes[1e6-1].next = &nodes[0];
    for (auto i = 0; i < cups.size()-1; ++i) nodes[cups[i]-1].next = &nodes[cups[i+1]-1];
    nodes[cups[cups.size()-1]-1].next = &nodes[cups.size()];
    nodes[1e6-1].next = &nodes[cups[0]-1];
    Node* curr = &nodes[cups[0]-1];
    for (int i = 0; i < 1e7; ++i) {
        Node* first = curr->next;
        Node* second = first->next;
        Node* third = second->next;
        curr->next = third->next;
        int dest = curr->val-1;
        if (dest == 0) dest = 1e6;
        while (dest == first->val || dest == second->val || dest == third->val) {
            dest -= 1;
            if (dest == 0) dest = 1e6;
        }
        third->next = nodes[dest-1].next;
        nodes[dest-1].next = first;
        curr = curr->next;
    }
    Node* muller = &nodes[0];
    long long mul = 1;
    for (auto i = 0; i < 2; ++i) {
        muller = muller->next;
        mul *= muller->val;
    }
    std::cout << mul << std::endl;
}

int main() {
    std::ifstream file{"/home/matoototo/Programming/aoc/23/input"};
    std::deque<int> cups = get_cups(file);
    p1(cups);
    p2(cups);
    return 0;
}