#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <cmath>
#include <utility>

std::vector<std::pair<int, int>> parse_ids(std::ifstream& file) {
    std::vector<std::pair<int, int>> ids;
    std::string s;
    getline(file, s); 
    getline(file, s);
    std::istringstream ss{s};
    int i = 0;
    while (getline(ss, s, ',')) {
        if (s != "x") ids.push_back(std::make_pair(std::stoi(s), std::stoi(s)-(i%std::stoi(s))));
        ++i;
    }
    return ids;
}

int find_closest(const std::vector<std::pair<int, int>>& ids, const int& earliest) {
    for (int i = 0; ; ++i) {
        for (auto id : ids) if ((earliest+i)%id.first == 0) return i*id.first;
    }
}

long long inverse_modulo(long long x, long long mod) { // too big for long long...
    std::cout << x << " " << mod << std::endl;
    return (long long)std::pow(x, mod-1)%mod;
}

int gcdExtended(long long a, long long b, long long *x, long long *y) {  
    if (a == 0) {  
        *x = 0;  
        *y = 1;  
        return b;  
    }
    long long x1, y1;
    int gcd = gcdExtended(b%a, a, &x1, &y1);  
    *x = y1 - (b/a) * x1;  
    *y = x1;
    return gcd;  
}


long long p2(std::vector<std::pair<int, int>> ids) {
    long long N = 1;
    std::vector<long long> xis;
    long long result = 0;
    for (auto id : ids) N *= id.first;
    for (auto id : ids) {
        long long N_i = N/id.first;
        long long x, y;
        gcdExtended(N_i, id.first, &x, &y);
        x = (id.first + (x%id.first))%id.first;
        result += x*N_i*id.second;
    }
    result %= N;
    return result;
}

int main() {
    std::ifstream file{"input"};
    int earliest;
    file >> earliest;
    auto ids = parse_ids(file);
    std::cout << find_closest(ids, earliest) << std::endl;
    std::cout << p2(ids) << std::endl;
    return 0;
}