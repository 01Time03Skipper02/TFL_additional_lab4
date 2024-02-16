#include "automat.cpp"
#include "generate_words.cpp"


void showRule(const Rule& r) {
    std::cout << r.left << " -> ";
    for (const auto& s : r.right) {
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
    std::set <char> reachable_terms;
    reachable_terms.insert('S');
    int set_size = reachable_terms.size();
    while(1) {
        for (auto rule : G) {
            if (reachable_terms.count(rule.left[0])) {
                std::vector <char> nonterms = getNonterminals(rule);
                for (auto term : nonterms) {
                    reachable_terms.insert(term);
                }
            }
        }
        if (reachable_terms.size() > set_size) {
            set_size = reachable_terms.size();
        }
        else break;
    }

    for (int i = 0; i < G.size(); i++) {
        if (!reachable_terms.count(G[i].left[0])) {
            G.erase(G.begin() + i);
            i--;
            continue;
        }
        std::vector <char> nonterms = getNonterminals(G[i]);
        for (auto term : nonterms) {
            if (!reachable_terms.count(term)) {
                G.erase(G.begin() + i);
                i--;
                break;
            }
        }
    }
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
    deleteNonAchievableRules(G);

    for (const auto& rule : G) {
        showRule(rule);
    }

    WordsGen generator(G);
    std::vector <std::string> words = generator.getWords(6);
    for (auto word: words) {
        std::cout << word << std::endl;
    }



    // regex to automaton
//    std::vector <std::pair <std::string, std::string > > lexemes = lexer(R);
//    std::vector <std::pair <std::string, std::string > > postfix = to_postfix(lexemes);
//    TreeNode* tree = build_tree(postfix);
//    auto res = regex_2_automato(tree);
//    res.show_automaton();
//    res.show_like_arrows();
//    auto res_addit = res.get_addition_automaton();
//    res_addit.show_automaton();
//    res_addit.show_like_arrows();

    return 0;
}
