#include <fstream>
#include <iostream>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <sstream>
#include <memory>

struct Node {
    std::vector<int> cords{0, 0, 0};
    std::unordered_map<std::string, std::shared_ptr<Node>> neighbours; // e, se, sw, w, nw, and ne
    bool is_white = true;
    Node(const std::vector<int>& c): cords(c) {}
    int x() {return cords[0];}
    int y() {return cords[1];}
    int z() {return cords[2];}
    int hash() {return 1000000*x() + 1000*y() + z();}
};

std::string rev(const std::string& s) {
    if (s == "e") return "w";
    if (s == "w") return "e";
    if (s == "se") return "nw";
    if (s == "nw") return "se";
    if (s == "sw") return "ne";
    if (s == "ne") return "sw";
    return "";
}

int x(const std::string& dir) {
    if (dir == "e") return 1;
    if (dir == "w") return -1;
    if (dir == "ne") return 1;
    if (dir == "sw") return -1;
    return 0;
}
int y(const std::string& dir) {
    if (dir == "se") return -1;
    if (dir == "nw") return 1;
    if (dir == "e") return -1;
    if (dir == "w") return 1;
    return 0;
}
int z(const std::string& dir) {
    if (dir == "ne") return -1;
    if (dir == "sw") return 1;
    if (dir == "se") return 1;
    if (dir == "nw") return -1;
    return 0;
}

std::shared_ptr<Node> find_tile(const std::unordered_map<int, std::shared_ptr<Node>>& nodes, std::shared_ptr<Node> add) {
    int hash = add->hash();
    auto it = nodes.find(hash);
    if (it == nodes.end()) return add;
    else add = it->second;
    return add;
}
std::unordered_map<int, std::shared_ptr<Node>> parse_file(std::ifstream& file) {
    std::string s;
    std::unordered_map<int, std::shared_ptr<Node>> nodes, ending;
    std::shared_ptr<Node> ref = std::make_shared<Node>(Node({0, 0, 0}));
    nodes.insert(std::make_pair(ref->hash(), ref));
    while (getline(file, s)) {
        std::vector<std::string> dirs;
        std::istringstream ss{s};
        std::string tmp;
        char c;
        while (ss >> c) {
            tmp.push_back(c);
            if (tmp[0] != 's' && tmp[0] != 'n' || tmp.size() == 2) {
                dirs.push_back(tmp);
                tmp.clear();
            }
        }
        std::shared_ptr<Node> curr = ref;
        for (auto& dir : dirs) {
            std::shared_ptr<Node> add = std::make_shared<Node>(Node({curr->x()+x(dir), curr->y()+y(dir), curr->z()+z(dir)}));
            add = find_tile(nodes, add);
            nodes.insert(std::make_pair(add->hash(), add));
            add->neighbours[rev(dir)] = curr;
            curr->neighbours[dir] = add;
            curr = curr->neighbours.find(dir)->second;
        }
        curr->is_white = !curr->is_white;
    }
    return nodes;
}


int p1(const std::unordered_map<int, std::shared_ptr<Node>>& nodes) {
    int black = 0;
    for (auto& node : nodes) black += !node.second->is_white;
    return black;
}

int p2(std::unordered_map<int, std::shared_ptr<Node>>& nodes, int times) {
    std::vector<std::string> dirs = {"e", "se", "sw", "w", "nw", "ne"};
    while (times != 0) {
        std::unordered_map<int, std::shared_ptr<Node>> nodes_new = nodes;
        for (auto& _ : nodes) {
            auto& node = _.second;
            for (auto& dir : dirs) {
                std::shared_ptr<Node> add = std::make_shared<Node>(Node({node->x()+x(dir), node->y()+y(dir), node->z()+z(dir)}));
                add = find_tile(nodes_new, add);
                nodes_new.insert(std::make_pair(add->hash(), add));
            }
        }
        nodes = nodes_new;
        std::vector<std::shared_ptr<Node>> change;
        for (auto& _ : nodes) {
            auto& node = _.second;
            int black = 0;
            for (auto& dir : dirs) {
                std::shared_ptr<Node> add = std::make_shared<Node>(Node({node->x()+x(dir), node->y()+y(dir), node->z()+z(dir)}));
                add = find_tile(nodes_new, add);
                black += !add->is_white;
            }
            if ((black == 0 || black > 2) && !node->is_white) change.push_back(node);
            if (black == 2 && node->is_white) change.push_back(node);
        }
        for (auto& node : change) node->is_white = !node->is_white;
        --times;
    }
    return p1(nodes);
}
int main() {
    std::ifstream file{"/home/matoototo/Programming/aoc/24/input"};
    auto nodes = parse_file(file);
    std::cout << p1(nodes) << std::endl;
    std::cout << p2(nodes, 100) << std::endl;
    return 0;
}