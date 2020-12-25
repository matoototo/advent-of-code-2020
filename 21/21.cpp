#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <sstream>
#include <algorithm>
#include <unordered_map>

struct Food {
    std::set<std::string> ings;
    std::set<std::string> allgs;
};

std::vector<Food> process_input(std::ifstream& file) {
    std::string s;
    std::vector<Food> foods;
    while (getline(file, s)) {
        Food food;
        std::istringstream ss{s};
        getline(ss, s, '(');
        std::istringstream ings{s};
        while (getline(ings, s, ' ')) food.ings.insert(s);
        getline(ss, s, ' ');
        while (getline(ss, s, ' ')) food.allgs.insert(s.substr(0, s.size()-1));
        foods.push_back(food);
    }
    return foods;
}

std::vector<Food> with_ing(const std::string& ingredient, const std::vector<Food>& foods) {
    std::vector<Food> foods_with;
    for (auto& food : foods) {
        if (food.ings.find(ingredient) != food.ings.end()) foods_with.push_back(food);
    }
    return foods_with;
}

std::vector<Food> with_allg(const std::string& allergen, const std::vector<Food>& foods) {
    std::vector<Food> foods_with;
    for (auto& food : foods) {
        if (food.allgs.find(allergen) != food.allgs.end()) foods_with.push_back(food);
    }
    return foods_with;
}

Food intersect_all(const std::vector<Food>& foods) {
    Food inter = foods[0];
    for (auto &food : foods) {
        std::set<std::string> tmp;
        std::set_intersection(inter.ings.begin(), inter.ings.end(), food.ings.begin(), food.ings.end(), std::inserter(tmp, tmp.begin()));
        inter.ings = tmp;
        tmp.clear();
        std::set_intersection(inter.allgs.begin(), inter.allgs.end(), food.allgs.begin(), food.allgs.end(), std::inserter(tmp, tmp.begin()));
        inter.allgs = tmp;
    }
    return inter;
}

std::set<std::string> all_ings(const std::vector<Food>& foods) {
    std::set<std::string> ings;
    for (auto& food : foods) std::set_union(ings.begin(), ings.end(), food.ings.begin(), food.ings.end(), std::inserter(ings, ings.begin()));
    return ings;
}

std::set<std::string> all_allgs(const std::vector<Food>& foods) {
    std::set<std::string> allgs;
    for (auto& food : foods) std::set_union(allgs.begin(), allgs.end(), food.allgs.begin(), food.allgs.end(), std::inserter(allgs, allgs.begin()));
    return allgs;
}

void remove_allg(std::vector<Food>& foods, const std::string& allg) {
    for (auto& food : foods) food.allgs.erase(allg);
}

void remove_ing(std::vector<Food>& foods, const std::string& ing) {
    for (auto& food : foods) food.ings.erase(ing);
}

std::unordered_map<std::string, std::string> eliminate_singles(std::vector<Food>& foods, const std::set<std::string>& allgs) {
    auto ings = all_ings(foods);
    std::unordered_map<std::string, std::string> ing_all;
    for (auto& ing : ings) {
        std::vector<Food> filtered = with_ing(ing, foods);
        for (auto& allg : allgs) {
            std::vector<Food> filtered_2 = with_allg(allg, filtered);
            if (!filtered_2.empty()) {
                Food intersection = intersect_all(filtered_2);
                if (intersection.ings.size() == 1) {
                    ing_all.insert(std::make_pair(ing, allg));
                    remove_allg(foods, allg);
                    remove_ing(foods, ing);
                }
            }
        }
    }
    return ing_all;
}
std::unordered_map<std::string, std::string> create_map(std::vector<Food> foods) {
    auto allgs = all_allgs(foods);
    std::unordered_map<std::string, std::string> ing_all;
    while (!allgs.empty()) {
        auto tmp = eliminate_singles(foods, allgs);
        ing_all.insert(tmp.begin(), tmp.end());
        allgs = all_allgs(foods);
    }
    return ing_all;
}

int p1(const std::vector<Food>& foods, const std::unordered_map<std::string, std::string>& ing_all) {
    auto ings = all_ings(foods);
    int count = 0;
    for (auto ing : ings) {
        if (ing_all.find(ing) == ing_all.end()) {
            for (auto food : foods) {
                count += (food.ings.find(ing) != food.ings.end());
            }
        }
    }
    return count;
}

std::string p2(std::unordered_map<std::string, std::string> ing_all) {
    std::string s;
    while (!ing_all.empty()) {
        std::string key = ing_all.begin()->first;
        std::string next = ing_all.begin()->second;
        for (auto pair : ing_all) {
            if (pair.second < next) {
                key = pair.first;
                next = pair.second;
            }
        }
        s += key + ',';
        ing_all.erase(key);
    }
    return s.substr(0, s.size()-1);
} 

int main() {
    std::ifstream file{"input"};
    auto foods = process_input(file);
    auto ing_all = create_map(foods);
    std::cout << p1(foods, ing_all) << std::endl;
    std::cout << p2(ing_all) << std::endl;
    return 0;
}