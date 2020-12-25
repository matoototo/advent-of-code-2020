#include <fstream>
#include <iostream>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <sstream>
#include <set>

struct Bag {
    std::string id;
    std::vector<Bag*> contents;
    Bag(std::string i): id(i) {} 
    bool contains_golden() {        
        bool contains = false;
        std::set<Bag*> unique(this->contents.begin(), this->contents.end()); 
        for (Bag* b : unique) {
            if (b->id == "shiny gold") return true;
            contains += b->contains_golden();
        }
        return contains;
    }
    int real_size() {
        int i = this->contents.size();
        for (Bag* b : this->contents) {
            i += b->real_size();
        }
        return i; 
    }
};

using umap = std::unordered_map<std::string, Bag*>;

umap parse_file(std::ifstream& file) { // returns a map of bags, one of each type
    umap bags;
    std::string s;
    Bag* bag;
    while (getline(file, s)) {
        std::string bag_id = s.substr(0, s.find("bag")-1);
        auto f = bags.find(bag_id);
        if (f == bags.end()) {
            bag = new Bag(bag_id);
            bags.insert({bag->id, bag});
        }
        else {
            bag = f->second;
        }
        if (s.find("no other bags.") != std::string::npos) continue;
        s = s.substr(bag->id.length()+14);
        std::istringstream ss{s};
        std::vector<std::string> split;
        while (std::getline(ss, s, ' ')) split.push_back(s);
        for (auto it = split.begin(); it != split.end(); ++it) {
            if (it->substr(0, 3) == "bag") {
                int num = std::stoi(*(it-3));
                std::string tmp_bag_id = *(it-2) + " " + *(it-1);
                auto f = bags.find(tmp_bag_id);
                if (f == bags.end()) {
                    Bag* ptr = new Bag(tmp_bag_id);
                    f = bags.insert({tmp_bag_id, ptr}).first;
                }
                for (int i = 0; i < num; ++i) bag->contents.push_back(f->second);
            }
        }
        
    }
    return bags;
}

int count_golden(umap& bags) {
    int n = 0;
    Bag* golden = bags.find("shiny gold")->second;
    for (auto bag : bags) n += bag.second->contains_golden();
    return n;
}

int main() {
    std::ifstream file{"input"};
    auto map = parse_file(file);
    std::cout << count_golden(map) << std::endl;
    std::cout << map.find("shiny gold")->second->real_size() << std::endl;
    return 0;
}
