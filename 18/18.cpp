#include <iostream>
#include <fstream>
#include <vector>
#include <deque>
#include <algorithm>
#include <sstream>

struct P {
    char c = '-';
    long long op;
    P(long long operand): op(operand) {}
    P(char operation): c(operation) {}
};


long long eval(long long p1, char op, long long p2) {
    if (op == '+') p1 += p2;
    else if (op == '*') p1 *= p2;
    else std::cout << "WHOOPS! " << op << std::endl;
    return p1;
}

bool smaller_precedence(char c1, char c2) {
    if (c1 == '+' && c2 == '*') return false;
    if (c1 == '*' && c2 == '+') return true;
    if (c1 == '(') return true;
    return false;
}

long long compute_expression(const std::string& ex) {
    long long n = 0;
    long long operand;
    bool first = true;
    std::vector<P> stack;
    for (auto it = ex.begin(); it < ex.end(); ++it) {
        if (*it == '+' || *it == '*') stack.emplace_back(*it);
        else if (*it == '(') {
            stack.emplace_back(n);
            first = true;
            n = 0;
        }
        else if (*it == ')') {
            operand = stack.back().op;
            stack.pop_back();
            if (stack.size() != 0 && stack.back().c != '-') {
                P op = stack.back();
                stack.pop_back();
                n = eval(n, op.c, operand);
            }
        }
        else {
            std::istringstream ss{std::string(it, ex.end())};
            ss >> operand;
            if (first) {
                n = operand;
                first = false;
            }
            else {
                P op = stack.back();
                n = eval(n, op.c, operand);
                stack.pop_back();
            }   
            std::string _ = std::to_string(operand); 
            it += _.size()-1; // otherwise breaks on >1 digit numbers
        }
    }
    return n;
}


long long compute_expression_2(const std::string& ex) {
    long long operand;
    std::vector<long long> operands;
    std::vector<char> operators;
    for (auto it = ex.begin(); it < ex.end(); ++it) {
        if (*it == '+' || *it == '*') {
            if (operators.size() == 0 || smaller_precedence(operators.back(), *it)) operators.push_back(*it);
            else {
                while (operators.size() > 0 && !smaller_precedence(operators.back(), *it)) {
                    long long op1 = operands.back();
                    operands.pop_back();
                    long long op2 = operands.back();
                    operands.pop_back();
                    operands.push_back(eval(op1, operators.back(), op2));
                    operators.pop_back();
                }
                operators.push_back(*it);
            }
        }
        else if (*it == '(') operators.push_back('(');
        else if (*it == ')') {
            while (operators.size() > 0 && operators.back() != '(') {
                long long op1 = operands.back();
                operands.pop_back();
                long long op2 = operands.back();
                operands.pop_back();
                operands.push_back(eval(op1, operators.back(), op2));
                operators.pop_back();
            }
            if (operators.back() == '(') operators.pop_back();
        }
        else {
            std::istringstream ss{std::string(it, ex.end())};
            ss >> operand;
            operands.push_back(operand);
            std::string _ = std::to_string(operand); 
            it += _.size()-1; // otherwise breaks on >1 digit numbers
        }
    }
    while (operators.size() != 0) {
        long long op1 = operands.back();
        operands.pop_back();
        long long op2 = operands.back();
        operands.pop_back();
        operands.push_back(eval(op1, operators.back(), op2));
        operators.pop_back();
    }
    return operands.back();
}

std::vector<std::string> parse_file(std::ifstream& file) {
    std::string s;
    std::vector<std::string> expressions;
    while (getline(file, s)) {
        s.erase(std::remove_if(s.begin(), s.end(), isspace), s.end());
        expressions.push_back(s);
    }
    return expressions;
}

long long compute_p1(const std::vector<std::string>& expressions) {
    long long sum = 0;
    for (std::string ex : expressions) sum += compute_expression(ex);
    return sum;
}

long long compute_p2(const std::vector<std::string>& expressions) {
    long long sum = 0;
    for (std::string ex : expressions) sum += compute_expression_2(ex);
    return sum;
}
int main() {
    std::ifstream file{"input"};
    auto expressions = parse_file(file);
    std::cout << compute_p1(expressions) << std::endl;
    std::cout << compute_p2(expressions) << std::endl;
    return 0;
}