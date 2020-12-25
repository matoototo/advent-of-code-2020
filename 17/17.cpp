#include <fstream>
#include <iostream>
#include <vector>
#include <deque>
#include <string>

struct World;


struct Layer {
    Layer* above = nullptr;
    Layer* below = nullptr;
    World* parent = nullptr;
    std::deque<std::deque<bool>> elements;
    Layer() {}
    Layer(const Layer &cp) {
        elements = cp.elements;
    }
    Layer* get_changes(bool hyper = false, int depth = 0);
    void zero_out();
    void add_row(std::string s, bool front = false) {
        std::deque<bool> r;
        for (auto c : s) r.push_back(c == '#');
        if (front) elements.push_front(r);
        else elements.push_back(r);
    }
    int active_neighbours(int x, int y, bool single = false, bool hyper = false) {
        int count = 0;
        for (int i = -1; i < 2; ++i) {
            for (int j = -1; j < 2; ++j) {
                count += elements[x+i][y+j];
            }
        }
        if (single) return count;
        if (!hyper) count -= elements[x][y];
        count += above->active_neighbours(x, y, true);
        count += below->active_neighbours(x, y, true);
        return count;
    }
};

struct World {
    World* nim = nullptr;
    World* bor = nullptr;
    std::deque<Layer*> world;
    World() {}
    World(const World &cp) {
        for (auto layer : cp.world) {
            Layer* foo = new Layer(*layer);
            if (world.size() != 0) {
                foo->below = world.back();
                world.back()->above = foo;
            }
            world.push_back(foo);
        }
    }
    void zero_out() {
        for (auto layer : world) layer->zero_out();
    }

};

Layer* Layer::get_changes(bool hyper, int depth) {
    Layer* changes = new Layer(*this);
    for (int i = 0; i < elements.size(); ++i) {
        for (int j = 0; j < elements[i].size(); ++j) {
            if (i == 0 || j == 0 || i == elements.size()-1 || j == elements[i].size()-1) changes->elements[i][j] = false;
            else {
                int count = active_neighbours(i, j);
                if (hyper) {
                    count += parent->bor->world[depth]->active_neighbours(i, j, false, true);
                    count += parent->nim->world[depth]->active_neighbours(i, j, false, true);
                }
                if (count == 3 || (count == 2 && elements[i][j])) changes->elements[i][j] = true;
                else changes->elements[i][j] = false;
            }
        }
    }
    return changes;
}

void Layer::zero_out() {
    for (auto &row : elements) {
        for (auto &cube : row) {
            cube = false;
        }
    }
}

void print_world(const std::deque<Layer*>& world);

World* parse_file(std::ifstream& file) {
    std::string s;
    World* world = new World();
    Layer* z0 = new Layer();
    z0->parent = world;
    while(getline(file, s)) {
        s = '.' + s + '.'; // padding
        z0->add_row(s);
    }
    s = std::string(s.size(), '.');
    z0->add_row(s);
    z0->add_row(s, true);
    world->world.push_back(z0);
    return world;
}

// void add_empty(World* world) {
//     for (auto layer : world->world) {
//         layer->elements.emplace_back(layer->elements.front().size(), 0);
//         layer->elements.emplace_front(layer->elements.front().size(), 0);
//         for (auto &row : layer->elements) {
//             row.push_back(0);
//             row.push_back(0);
//             row.push_front(0);
//             row.push_front(0);
//         }
//     }
//     Layer* top = new Layer();
//     Layer* bottom = new Layer();
//     top->elements = world->world.back()->elements;
//     bottom->elements = world->world.front()->elements;
//     for (int i = 0; i < top->elements.size(); ++i) {
//         for (int j = 0; j < top->elements[i].size(); ++j) {
//             top->elements[i][j] = false;
//             bottom->elements[i][j] = false;
//         }
//     }
//     world->world.back()->above = top;
//     world->world.front()->below = bottom;
//     top->below = world->world.back();
//     bottom->above = world->world.front();
//     world->world.push_back(top);
//     world->world.push_front(bottom);
// }

// void simulate(World* world) {
//     std::deque<Layer*> changes;
//     add_empty(world);
//     add_empty(world);
//     for (int i = 1; i < world->world.size()-1; ++i) {
//         changes.push_back(world->world[i]->get_changes());
//         if (i != 1) {
//             changes[i-2]->above = changes.back();
//             changes[i-1]->below = changes[i-2];
//         }
//     }
//     world->world = changes;
// }

void add_empty(std::deque<World*>& hyper) {
    for (auto &world : hyper) {
        for (auto &layer : world->world) {
            layer->elements.emplace_back(layer->elements.front().size(), 0);
            layer->elements.emplace_front(layer->elements.front().size(), 0);
            for (auto &row : layer->elements) {
                row.push_back(0);
                row.push_back(0);
                row.push_front(0);
                row.push_front(0);
            }
        }
        Layer* top = new Layer();
        Layer* bottom = new Layer();
        top->parent = world;
        bottom->parent = world;
        top->elements = world->world.back()->elements;
        bottom->elements = world->world.front()->elements;
        for (int i = 0; i < top->elements.size(); ++i) {
            for (int j = 0; j < top->elements[i].size(); ++j) {
                top->elements[i][j] = false;
                bottom->elements[i][j] = false;
            }
        }
        world->world.back()->above = top;
        world->world.front()->below = bottom;
        top->below = world->world.back();
        bottom->above = world->world.front();
        world->world.push_back(top);
        world->world.push_front(bottom);
    }
    World* nim = new World(*hyper.back());
    for (auto &layer : nim->world) layer->parent = nim;
    World* bor = new World(*hyper.front());
    for (auto &layer : bor->world) layer->parent = bor;
    hyper.back()->nim = nim;
    hyper.front()->bor = bor;
    nim->bor = hyper.back();
    bor->nim = hyper.front();
    nim->zero_out();
    bor->zero_out();
    hyper.push_back(nim);
    hyper.push_front(bor);

}

void simulate(std::deque<World*>& hyper) {
    std::deque<World*> changes;
    add_empty(hyper);
    add_empty(hyper);
    for (int i = 1; i < hyper.size()-1; ++i) {
        World* change = new World();
        for (int j = 1; j < hyper[i]->world.size()-1; ++j) {
            change->world.push_back(hyper[i]->world[j]->get_changes(true, j));
            if (j != 1) {
                change->world[j-2]->above = change->world.back();
                change->world[j-1]->below = change->world[j-2];
            }
        }
        changes.push_back(change);
        if (i != 1) {
            changes[i-2]->nim = changes.back();
            changes[i-1]->bor = changes[i-2];
        }
    }
    for (auto &w : changes) {
        for (auto &layer : w->world) layer->parent = w;
    }
    hyper = changes;
}

int count_active(World* world) {
    int active = 0;
    for (auto layer : world->world) {
        for (int i = 0; i < layer->elements.size(); ++i) {
            for (int j = 0; j < layer->elements[i].size(); ++j) {
                active += layer->elements[i][j];
            }
        }
    }
    return active;
}

int count_active_hyper(std::deque<World*> hyper) {
    int active = 0;
    for (auto world : hyper) active += count_active(world);
    return active;
}

// void print_world(const std::deque<Layer*>& world) {
//     for (auto layer : world) {
//         for (int i = 0; i < layer->elements.size(); ++i) {
//             for (int j = 0; j < layer->elements[i].size(); ++j) {
//                 if (layer->elements[i][j]) std::cout << '#';
//                 else std::cout << '.';
//             }
//             std::cout << '\n';
//         }
//         std::cout << '\n';
//     }
//     std::cout << "-----------------\n";
// }

int main() {
    std::ifstream file{"input"};
    std::deque<World*> hyper;
    World* world = parse_file(file);
    hyper.push_back(world);
    simulate(hyper);
    simulate(hyper);
    simulate(hyper);
    simulate(hyper);
    simulate(hyper);
    simulate(hyper);;
    std::cout << count_active_hyper(hyper) << std::endl;
    return 0;
}