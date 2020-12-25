#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <algorithm>
#include <unordered_set>

struct Rule {
    int id;
    std::vector<std::vector<Rule*>> sub_rules;
    char letter = '-';
    std::vector<std::string> matches;
    bool done = false;
    Rule(int n): id(n) {}
    std::vector<std::string> find_matches() {
        if (done) {
            return matches;
        }
        if (letter != '-') {
            matches.emplace_back(1, letter);
            done = true;
            return matches;
        }
        for (auto &sub : sub_rules) {
            std::vector<std::string> sub_matches;
            for (auto &rule : sub) {
                if (sub_matches.size() == 0) sub_matches = rule->find_matches();
                else {
                    std::vector<std::string> new_sub_matches;
                    auto branch_matches = rule->find_matches();
                    for (auto &s1 : sub_matches) {
                        for (auto &s2 : branch_matches) {
                            new_sub_matches.push_back(s1+s2);
                        } 
                    }
                    sub_matches = new_sub_matches;
                }
            }
            for (auto &s : sub_matches) matches.push_back(s);
        }
        done = true;
        return matches;
    }
};

std::vector<Rule*> parse_file(std::ifstream& file) {
    std::vector<Rule*> rules;
    std::vector<int> ids;
    std::string s;
    while(getline(file, s) && s != "") {
        int id; std::istringstream ss{s};
        ss >> id;
        ids.push_back(id);
    }
    std::sort(ids.begin(), ids.end());
    for (auto id : ids) {
        Rule* foo = new Rule(id);
        rules.push_back(foo);
    }
    file.clear();
    file.seekg(0);
    while(getline(file, s) && s != "") {
        int id; std::istringstream ss{s};
        ss >> id;
        std::string tmp;
        getline(ss, tmp, ' ');
        getline(ss, tmp, ' ');
        if (tmp[0] == '\"') {
            rules[id]->letter = tmp[1];
            continue;
        }
        std::vector<Rule*> match;
        match.push_back(rules[std::stoi(tmp)]);
        while (getline(ss, tmp, ' ')) {
            if (tmp == "|") {
                rules[id]->sub_rules.push_back(match);
                match.clear();
            }
            else match.push_back(rules[std::stoi(tmp)]);
        }
        rules[id]->sub_rules.push_back(match);
    }
    return rules;
}

std::vector<std::string> split_message(std::string message) {
    std::vector<std::string> split;
    for (int i = 0; i < message.size(); i += 8) {
        split.push_back(message.substr(i, 8));
    }
    return split;
}

int count_matching(const std::vector<Rule*>& rules, std::ifstream& file, bool p2 = false) {
    std::string s;
    std::vector<std::string> messages;
    int max_length = 0;
    while(getline(file, s)) {
        if (s.size() > max_length) max_length = s.size();
        messages.push_back(s);
    }
    int n = 0;
    auto tmp = rules[0]->find_matches();
    std::unordered_set<std::string> matches{tmp.begin(), tmp.end()};
    std::unordered_set<std::string> words_42{rules[42]->matches.begin(), rules[42]->matches.end()};
    std::unordered_set<std::string> words_31{rules[31]->matches.begin(), rules[31]->matches.end()};
    int goods = 0;
    if (p2) {
        for (auto m : messages) {
            if (m.size() <= 24) continue;
            auto split = split_message(m);
            bool good = true;
            bool frty2 = true;
            int nfrty2 = 0;
            int ntrty1 = 0;
            for (auto& word : split) {
                if (frty2) {
                    if (words_42.find(word) == words_42.end()) frty2 = false;
                    else nfrty2++;
                }
                if (!frty2) {
                    if (words_31.find(word) == words_31.end()) {
                        good = false;
                        break;
                    }
                    else ntrty1++;
                }
            }
            goods += (good&&!(frty2)&&(nfrty2>ntrty1));
        } 
    }
    for (auto m : messages) {
        n += matches.find(m) != matches.end();
    }
    return n+goods;
} 

int main() {
    std::ifstream file{"input"};
    auto rules = parse_file(file);
    // std::cout << count_matching(rules, file) << std::endl; 
    std::cout << count_matching(rules, file, true) << std::endl;
    return 0;
}