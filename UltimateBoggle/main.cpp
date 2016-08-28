#include "dictionary.hpp"
#include "compile.hpp"
#include "utils.hpp"
#include "board.hpp"
#include "solver_dfs.hpp"

#include <iostream>
#include <algorithm>
#include <cassert>
#include <cctype>
#include <chrono>

int main () try {
    using namespace ultimate_boggle;    
    dictionary s_dictionary ("dict.txt");    
    board s_board ("board.txt");
    solver_dfs s_solver (s_dictionary);
    std::vector<const char*> s_wordlist;    
    //TIMEIT (
    for (auto i = 0; i < 20; ++i) {
        s_solver.solve (s_board, s_wordlist);
        s_wordlist.clear ();
    }
    //);
    //for (const auto& s_word : s_wordlist) {
    //    std::cout << s_word << "\n";
    //}
    
    return 0;
}
catch (const std::exception& ex) {
    std::cout << ex.what () << "\n";
    return -1;
}