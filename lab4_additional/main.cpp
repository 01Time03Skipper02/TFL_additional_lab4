#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <cassert>

struct Rule {
    std::string left;
    std::vector <std::string> right;
};

int main() {
    std::ifstream fin("input.txt");
    assert(fin.is_open());
    std::string input;
    std::string R;
    std::vector <Rule> G;
    fin >> R;
    std::string left;
    std::vector <std::string> right;
    std::string s;
    getline(fin, input);
    while(getline(fin, input)) {
        if (input.empty()) continue;
        std::cout << input << std::endl;
        std::stringstream ss(input);
        ss >> left;
        ss >> s; // skip "->"
        while(ss >> s) {
            right.emplace_back(s);
        }
        G.push_back({left, right});
        right.clear();
    }
    return 0;
}
