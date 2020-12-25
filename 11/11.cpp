#include <fstream>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <utility>

struct Seat {
    bool occupied = false;
    bool is_seat = false;
    std::pair<int, int> xy;
    std::vector<std::pair<int, Seat*>> neighbours;

    int adjacent() {
        int i = 0;
        for (auto& neighbour : neighbours) if (neighbour.second->occupied) ++i;
        return i;
    }

    int in_direction(int direction) {
        for (auto& neighbour : neighbours) {
            if (neighbour.first == direction) {
                if (neighbour.second->is_seat) return neighbour.second->occupied;
                else return neighbour.second->in_direction(direction);
            }
        }
        return 0;
    }

    int in_los() {
        int n = 0;
        for (int i = 0; i <= 7; ++i) n += this->in_direction(i);
        return n;
    }
};

std::unordered_map<int, Seat*> parse_file(std::ifstream& file) {
    std::vector<std::vector<char>> matrix;
    std::string str, x;
    while (getline(file, str)) {
        if (matrix.size() == 0) {
            x = std::string(str.size()+2, 'W');
            matrix.emplace_back(x.begin(), x.end());
        }
        str.insert(0, 1, 'W'); // padding
        str.push_back('W');
        matrix.emplace_back(str.begin(), str.end());
    }
    matrix.emplace_back(x.begin(), x.end());

    std::unordered_map<int, Seat*> seats;
    int size = x.size();
    for (int i = 0; i < matrix.size(); ++i) {
        for (int j = 0; j < matrix[i].size(); ++j) {
            if (matrix[i][j] != 'W') {
                Seat* s = new Seat();
                s->xy = std::make_pair(j, i);
                if (matrix[i][j] == 'L') s->is_seat = true;

                if (matrix[i-1][j] != 'W') {
                    s->neighbours.emplace_back(0, seats[(i-1)*size+j]);
                    seats[(i-1)*size+j]->neighbours.emplace_back(4, s);
                }
                if (matrix[i-1][j-1] != 'W') {
                    s->neighbours.emplace_back(1, seats[(i-1)*size+j-1]);
                    seats[(i-1)*size+j-1]->neighbours.emplace_back(5, s);
                } 
                if (matrix[i-1][j+1] != 'W') {
                    s->neighbours.emplace_back(2, seats[(i-1)*size+j+1]);
                    seats[(i-1)*size+j+1]->neighbours.emplace_back(6, s);
                }
                if (matrix[i][j-1] != 'W') {
                    s->neighbours.emplace_back(3, seats[i*size+j-1]);
                    seats[i*size+j-1]->neighbours.emplace_back(7, s);
                }
                seats[i*size+j] = s;
            }
        }
    }
    return seats;
}

bool change_state(const std::unordered_map<int, Seat*>& seats, bool p2 = false) {
    std::vector<Seat*> change;
    int upper = p2 ? 5 : 4;
    int n;
    int i = 0;
    for (auto& seat : seats) {
        if (seat.second->is_seat) {
            ++i;
            if (!p2) n = seat.second->adjacent();
            else n = seat.second->in_los();
            if (n == 0 && !seat.second->occupied) change.push_back(seat.second);
            if (n >= upper && seat.second->occupied) change.push_back(seat.second);
        }
    }
    if (change.size() == 0) return false;
    for (auto& seat : change) seat->occupied = !seat->occupied;
    return true;
}

int count_occupied(const std::unordered_map<int, Seat*>& seats) {
    int n = 0;
    for (auto& seat : seats) n += seat.second->occupied;
    return n;
}

int main() {
    std::ifstream file{"input"};
    auto seats = parse_file(file);
    // while (change_state(seats)); // part 1
    while (change_state(seats, true)); // part 2
    std::cout << count_occupied(seats) << std::endl;
    return 0;
}