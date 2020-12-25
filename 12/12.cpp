#include <vector>
#include <fstream>
#include <iostream>
#include <cmath>

struct Instruction {
    int x, y, angle, move;
    Instruction(int x, int y, int angle, int move): x(x), y(y), angle(angle), move(move) {};
};

struct Ship {
    int x = 0, y = 0, angle = 0;
    int manhattan() { return std::abs(x) + std::abs(y); }
    void move_p1(std::vector<Instruction> instructions) {
        for (auto inst : instructions) {
            this->angle = (this->angle+inst.angle)%360;
            double radians = M_PI*this->angle/180;
            this->x += inst.x + inst.move*(int)std::cos(radians);
            this->y += inst.y + inst.move*(int)std::sin(radians);
        }
    }
    void move_p2(std::vector<Instruction> instructions) {
        int dir_x = 10, dir_y = 1;
        for (auto inst : instructions) {;
            double radians = M_PI*inst.angle/180;
            int tmp = dir_x;
            dir_x = dir_x*(int)std::cos(radians)-dir_y*(int)std::sin(radians);
            dir_y = tmp*(int)std::sin(radians)+dir_y*(int)std::cos(radians);
            dir_x += inst.x;
            dir_y += inst.y;
            this->x += inst.move*dir_x;
            this->y += inst.move*dir_y;
        }
    }
};

std::vector<Instruction> process_input(std::ifstream& file) {
    std::string s;
    std::vector<Instruction> instructions;
    while (getline(file, s)) {
        int amount = std::stoi(s.substr(1));
        if (s[0] == 'N') instructions.emplace_back(0, amount, 0, 0);
        else if (s[0] == 'E') instructions.emplace_back(amount, 0, 0, 0);
        else if (s[0] == 'S') instructions.emplace_back(0, -amount, 0, 0);
        else if (s[0] == 'W') instructions.emplace_back(-amount, 0, 0, 0);
        else if (s[0] == 'F') instructions.emplace_back(0, 0, 0, amount);
        else if (s[0] == 'R') instructions.emplace_back(0, 0, -amount, 0);
        else if (s[0] == 'L') instructions.emplace_back(0, 0, amount, 0);
    };
    return instructions;
}

int main() {
    std::ifstream file{"input"};
    auto instructions = process_input(file);
    Ship p1;
    p1.move_p1(instructions);
    std::cout << p1.manhattan() << std::endl;
    Ship p2;
    p2.move_p2(instructions);
    std::cout << p2.manhattan() << std::endl;
    return 0;
}