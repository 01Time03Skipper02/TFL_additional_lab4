#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <cassert>
#include <set>

struct Rule {
    std::string left;
    std::vector <std::string> right;
};

void showRule(const Rule& r) {
    std::cout << r.left << " -> ";
    for (auto s : r.right) {
        std::cout << s << " ";
    }
    std::cout << std::endl;
}

std::vector <char> getNonterminals(const Rule& r) {
    std::vector <char> res;
    for (auto s : r.right) {
        if (s.size() == 1 && isupper(s[0])) {
            res.push_back(s[0]);
        }
    }
    return res;
}

void deleteNonGenerativeRules(std::vector <Rule>& G) {
    std::set <char> generative_rules;
    for (auto rule : G) {
        if (getNonterminals(rule).empty()) {
            generative_rules.insert(rule.left[0]);
        }
    }

    while(1) {
        bool end_f = true;
        for (auto rule : G) {
            std::vector <char> nonterms = getNonterminals(rule);
            int f = 0;
            for (auto term : nonterms) {
                if (!generative_rules.count(term)){
                    f = 1;
                    break;
                }
            }
            if (f == 0 && !generative_rules.count(rule.left[0])) {
                end_f = false;
                generative_rules.insert(rule.left[0]);
            }
        }
        if (end_f) break;
    }

    for (int i = 0; i < G.size(); i++) {
        if (!generative_rules.count(G[i].left[0])) {
            G.erase(G.begin() + i);
            i--;
            continue;
        }
        std::vector <char> nonterms = getNonterminals(G[i]);
        for (auto term : nonterms) {
            if (!generative_rules.count(term)) {
                G.erase(G.begin() + i);
                i--;
                break;
            }
        }
    }
}

void deleteNonAchievableRules(std::vector <Rule>& G) {

}

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
        std::stringstream ss(input);
        ss >> left;
        ss >> s; // skip "->"
        while(ss >> s) {
            right.emplace_back(s);
        }
        G.push_back({left, right});
        right.clear();
    }
    deleteNonGenerativeRules(G);
    for (auto rule : G) {
        showRule(rule);
    }
    return 0;
}
