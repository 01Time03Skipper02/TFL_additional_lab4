#include "main_algo.cpp"

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

    for (auto rule : G) {
        showRule(rule);
    }

    // regex to automaton
    std::vector <std::pair <std::string, std::string > > lexemes = lexer(R);
    std::vector <std::pair <std::string, std::string > > postfix = to_postfix(lexemes);
    TreeNode* tree = build_tree(postfix);

//    auto res = regex_2_automato(tree);
//    res.show_automaton();
//    res.show_like_arrows();
//    auto res_addit = res.get_addition_automaton();
//    res_addit.show_automaton();
//    res_addit.show_like_arrows();
    automaton res = automaton({1, 0}, {{{"a", false}, {"b", false}}, {{"0", false}, {"b", false}}}, {0, 1});
    auto res_reg = main_algo(res, G);

    return 0;
}
