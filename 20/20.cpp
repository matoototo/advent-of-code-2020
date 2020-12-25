#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <memory>

struct Tile {
    int id;
    std::vector<std::string> data;
    std::vector<std::shared_ptr<Tile>> lneighbours;
    std::vector<std::shared_ptr<Tile>> tneighbours;
    std::vector<std::shared_ptr<Tile>> rneighbours;
    std::vector<std::shared_ptr<Tile>> bneighbours;
    std::string l, r;
    std::shared_ptr<Tile> rotate() {
        auto new_tile = std::make_shared<Tile>();
        new_tile->id = id;
        std::vector<std::string> new_data{data.size(), ""};
        for (int i = 0; i < data[0].size(); ++i) {
            for (int j = data.size()-1; j >= 0; --j) {
                new_data[i].push_back(data[j][i]);
            }
        }
        new_tile->data = new_data; 
        return new_tile;
    }
    std::shared_ptr<Tile> flip() { // vertically
        auto new_tile = std::make_shared<Tile>();
        new_tile->id = id;
        std::vector<std::string> new_data;
        for (int i = data.size()-1; i >= 0; --i) new_data.push_back(data[i]);
        new_tile->data = new_data;
        return new_tile;
    }
    std::string& top() { return data.front(); }
    std::string& bot() { return data.back(); }
    std::string& left() { 
        if (l == "") l = this->rotate()->data.front();
        return l;
    }
    std::string& right() { 
        if (r == "") r = this->rotate()->data.back();
        return r; 
    }

    std::vector<std::string> trim() {
        std::vector<std::string> trimmed;
        for (int i = 1; i < data.size()-1; ++i) {
            trimmed.push_back(data[i].substr(1, data[i].size()-2));
        }
        return trimmed;
    }
    void print() { for (auto& row : data) std::cout << row << std::endl; }
};

std::vector<std::shared_ptr<Tile>> parse_file(std::ifstream& file) {
    std::vector<std::shared_ptr<Tile>> tiles;
    std::string s;
    auto t = std::make_shared<Tile>();
    while (getline(file, s)) {
        if (s[0] == 'T') {
            std::istringstream ss{s};
            getline(ss, s, ' ');
            getline(ss, s, ':');
            t->id = std::stoi(s);
        }
        else if (s == "") {
            tiles.push_back(t);
            t = std::make_shared<Tile>();
        }
        else {
            t->data.push_back(s);
        }
    }
    tiles.push_back(t);
    return tiles;
}

std::vector<std::shared_ptr<Tile>> populate_transforms(std::vector<std::shared_ptr<Tile>>& tiles) {
    std::vector<std::shared_ptr<Tile>> all_tiles;
    for (auto& tile : tiles) {
        auto trans = tile;
        all_tiles.push_back(tile);
        for (int _ = 0; _ < 3; ++_) {
            all_tiles.push_back(trans->flip());
            trans = trans->rotate();
            all_tiles.push_back(trans);
        }
        all_tiles.push_back(trans->flip());
    }
    return all_tiles;
}

void populate_neighbours(std::vector<std::shared_ptr<Tile>>& tiles) {
    for (auto it = tiles.begin(); it < tiles.end(); ++it) {
        auto& tile = *it;
        auto tl = tile->left(), tr = tile->right(), tt = tile->top(), tb = tile->bot();
        for (auto it2 = it+1; it2 < tiles.end(); ++it2) {
            auto& check = *it2;
            if (tile->id != check->id) {
                if (tl == check->right()) {
                    tile->lneighbours.push_back(check);
                    check->rneighbours.push_back(tile);
                }
                if (tr == check->left()) {
                    tile->rneighbours.push_back(check);
                    check->lneighbours.push_back(tile);
                } 
                if (tt == check->bot()) {
                    tile->tneighbours.push_back(check);
                    check->bneighbours.push_back(tile);
                } 
                if (tb == check->top()) {
                    tile->bneighbours.push_back(check);
                    check->tneighbours.push_back(tile);
                } 
            }
        }
    }
}

int find_sea_monsters(std::shared_ptr<Tile> image) {
    std::vector<int> r1 = {18};
    std::vector<int> r2 = {0, 5, 6, 11, 12, 17, 18, 19};
    std::vector<int> r3 = {1, 4, 7, 10, 13, 16};
    int num = 0;
    for (int i = 0; i < image->data.size()-2; ++i) {
        for (int j = 19; j < image->data[i].size(); ++j) {
            bool is_monster = true;
            for (auto loc : r1) is_monster &= image->data[i][loc+j-19] == '#';
            for (auto loc : r2) is_monster &= image->data[i+1][loc+j-19] == '#';
            for (auto loc : r3) is_monster &= image->data[i+2][loc+j-19] == '#';
            num += is_monster;
        }
    }
    return num;
}

int count_hash(std::shared_ptr<Tile> image) {
    int n = 0;
    for (auto& row : image->data) {
        for (auto& c : row) n += c == '#';
    } 
    return n;
}

std::shared_ptr<Tile> reconstruct_image(const std::shared_ptr<Tile>& top_left) {
    std::vector<std::string> data;
    std::vector<std::shared_ptr<Tile>> left_column{top_left};
    std::shared_ptr<Tile> current = top_left;
    while (current->bneighbours.size() > 0) {
        current = current->bneighbours[0];
        left_column.push_back(current);
    }
    for (auto& row : left_column) {
        current = row;
        std::vector<std::string> section_rows = current->trim();
        while (current->rneighbours.size() > 0) {
            current = current->rneighbours[0];
            auto tmp_rows = current->trim();
            for (int i = 0; i < section_rows.size(); ++i) {
                section_rows[i] += tmp_rows[i];
            }
        }
        for (auto& sec_row : section_rows) data.push_back(sec_row);
    }
    auto image = std::make_shared<Tile>();
    image->data = data;
    return image;
}


int main() {
    std::ifstream file{"/home/matoototo/Programming/aoc/20/input"};
    auto tiles = parse_file(file);
    tiles = populate_transforms(tiles);
    populate_neighbours(tiles);
    std::shared_ptr<Tile> t;
    long long p1 = 1;
    std::vector<int> p1_ids;
    std::vector<std::shared_ptr<Tile>> p2_tiles;
    for (auto& tile : tiles) {
        int i = 0;
        if (tile->lneighbours.size() > 0) i += 1;
        if (tile->rneighbours.size() > 0) i += 1;
        if (tile->tneighbours.size() > 0) i += 1;
        if (tile->bneighbours.size() > 0) i += 1;
        if (i == 2 && std::find(p1_ids.begin(), p1_ids.end(), tile->id) == p1_ids.end()) {
            p1 *= tile->id;
            p1_ids.push_back(tile->id);
        }
        if (i == 2 && tile->rneighbours.size() == 1 && tile->bneighbours.size() == 1) {
            p2_tiles.push_back(tile);
        }
    }
    std::cout << p1 << std::endl;
    int p2 = -1;
    for (auto& tile : p2_tiles) {
        auto image = reconstruct_image(tile);
        int tmp = count_hash(image) - 15*find_sea_monsters(image);
        if (tmp < p2 || p2 == -1) p2 = tmp;
    }
    std::cout << p2 << std::endl;
    return 0;
}