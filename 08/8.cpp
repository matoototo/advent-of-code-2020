#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <string>
#include <sstream>

struct Command {
    int opcode, ex_count;
    std::vector<int> args;
    Command(int op, std::vector<int> a): opcode(op), args(a) {};
};

struct CPU {
    int acc = 0;
    bool execute(const std::vector<Command>& p) {
        std::vector<Command> prog{p.begin(), p.end()};
        for (auto i = 0; i != prog.size(); ++i) {
            Command& c = prog[i];
            if (c.ex_count != 0) return false;
            if (c.opcode == 1) i += c.args[0]-1;
            else if (c.opcode == 2) acc += c.args[0];
            c.ex_count += 1;
        }
        return true;
    }
};

std::vector<Command> parse_file(std::ifstream& file, std::unordered_map<std::string, int> s_opc) {
    std::vector<Command> commands;
    std::string s, arg, c;
    while (getline(file, s)) {
        std::istringstream ss{s};
        getline(ss, s, ' ');
        getline(ss, arg);
        if (arg[0] == '+') arg = arg.substr(1);
        std::vector<int> args;
        args.push_back(std::stoi(arg));
        commands.emplace_back(s_opc[s], args);
    }
    return commands;
}

int fix_broken(std::vector<Command>& prog) {
    CPU ceepeeuu;
    for (auto it = prog.begin(); it != prog.end(); ++it) {
        if (it->opcode == 1 || it->opcode == 0) {
            it->opcode = std::abs(it->opcode-1);
            if (ceepeeuu.execute(prog)) return ceepeeuu.acc;
            it->opcode = std::abs(it->opcode-1);
            ceepeeuu.acc = 0;
        }
    }
    return -1;
}

int main() {
    std::ifstream file{"input"};
    std::unordered_map<std::string, int> s_opc{{"nop", 0}, {"jmp", 1}, {"acc", 2}};
    auto commands = parse_file(file, s_opc);
    CPU ceepeeuu;
    ceepeeuu.execute(commands); // part 1
    std::cout << ceepeeuu.acc << std::endl;
    std::cout << fix_broken(commands) << std::endl; // part 2
    return 0;
}