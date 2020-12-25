#include <fstream>
#include <iostream>
#include <vector>
#include <deque>
#include <unordered_set>
#include <unordered_map>
#include <string>
#include <sstream>
#include <utility>

struct Fields {
    std::vector<std::unordered_set<int>> field_reqs;
    void add_field(int l1, int h1, int l2, int h2) {
        std::unordered_set<int> field;
        for (int i = l1; i <= h1; ++i) field.insert(i);
        for (int i = l2; i <= h2; ++i) field.insert(i); 
        field_reqs.push_back(field);
    }
};

struct Field {
    int value;
    std::deque<bool> in_range;
    Field(int val): value(val) {}
};

struct Ticket {
    std::vector<Field> fields;
};


Fields parse_reqs(std::ifstream& file) {
    std::string s;
    Fields pattern;
    while (getline(file, s), s != "") {
        std::istringstream ss{s};
        getline(ss, s, ':'); // name
        getline(ss, s, '-');
        int l1 = std::stoi(s);
        getline(ss, s, 'o');
        int h1 = std::stoi(s);
        getline(ss, s, ' ');
        getline(ss, s, '-');
        int l2 = std::stoi(s);
        getline(ss, s);
        int h2 = std::stoi(s);
        pattern.add_field(l1, h1, l2, h2);
    }
    return pattern;
}

Ticket parse_ticket(std::ifstream& file) {
    std::string s;
    Ticket ticket;
    getline(file, s);
    std::istringstream ss{s};
    while (getline(ss, s, ',')) {
        ticket.fields.emplace_back(std::stoi(s));
    }
    return ticket;
}

int find_error_rate(std::ifstream& file, const Fields& reqs, std::vector<Ticket>& good) {
    int er = 0;
    while (file) {
        Ticket t = parse_ticket(file);
        if (t.fields.size() == 0) return er;
        bool all_good = true;
        for (auto &field : t.fields) {
            bool is_good = false;
            for (auto &req : reqs.field_reqs) {
                bool in_range = (req.find(field.value) != req.end());
                is_good |= in_range;
                field.in_range.push_back(in_range);
            }
            if (!is_good) er += field.value;
            all_good &= is_good;
        }
        if (all_good) good.push_back(t);
    }
    return er;
}

std::unordered_map<int, int> find_order(const std::vector<Ticket>& good) {
    std::unordered_map<int, int> order;
    std::unordered_set<int> indices;
    bool any = true;
    while (any) {
        any = false;
        for (int j = 0; j < good[0].fields.size(); ++j) {
            if (order[j] != 0) continue;
            any = true;
            auto good_vec = good[0].fields[j].in_range;
            for (auto ticket : good) {
                for (int i = 0; i < ticket.fields[j].in_range.size(); ++i) {
                    good_vec[i] &= ticket.fields[j].in_range[i];
                }
            }
            int ones = 0;
            int index = -1;
            for (int i = 0; i < good_vec.size(); ++i) {
                if (indices.find(i) != indices.end()) continue;
                if (good_vec[i]) {
                    ++ones;
                    index = i;
                }
            }
            if (ones == 1) {
                order[j] = index+1;
                indices.insert(index);
            }
        }
    }
    return order;
}

long long p2(const std::unordered_map<int, int>& order, const Ticket& mine) {
    long long n = 1;
    for (int i = 0; i < mine.fields.size(); ++i) {
        if (order.at(i) <= 6) {
            n *= mine.fields[i].value;
        }
    }
    return n;
}

int main() {
    std::string _;
    std::ifstream file{"input"};
    Fields reqs = parse_reqs(file);
    getline(file, _);
    Ticket mine = parse_ticket(file);
    getline(file, _);
    getline(file, _);
    std::vector<Ticket> good;
    std::cout << find_error_rate(file, reqs, good) << std::endl;
    auto order = find_order(good);
    std::cout << p2(order, mine) << std::endl;
    return 0;
}