#include "dictionary.hpp"
#include "compile.hpp"
#include "utils.hpp"
#include "board.hpp"
#include "solver_dfs.hpp"
#include "solver_recursive.hpp"

#include <iostream>
#include <algorithm>
#include <cassert>
#include <cctype>
#include <chrono>

int main () try {
    using namespace ultimate_boggle;    
    dictionary s_dictionary ("dict.txt");    
    board s_board ("board.txt");    
/*
    std::vector<const char*> s_wordlist0;
    std::vector<const char*> s_wordlist1;

    solver_dfs       s_solver_dfs (s_dictionary);
    solver_recursive s_solver_recursive (s_dictionary);

    TIMEIT (s_solver_dfs.solve (s_board, s_wordlist0));
    TIMEIT (s_solver_recursive.solve (s_board, s_wordlist1));

    assert (s_wordlist0.size () == s_wordlist1.size ());

    std::sort (s_wordlist0.begin (), s_wordlist0.end (), [] (auto a, auto b) {
        return std::string(a) < std::string(b);
    });
    std::sort (s_wordlist1.begin (), s_wordlist1.end (), [] (auto a, auto b) {
        return std::string (a) < std::string (b);
    });

    for (auto i = 0; i < s_wordlist0.size (); ++i) {
        std::cout << s_wordlist0 [i] << "\n";
        assert (s_dictionary.match (s_wordlist0 [i]) == dictionary::match_type_full);
        assert (s_wordlist0 [i] == s_wordlist1 [i]);
    }
*/    

    std::vector<const char*> s_wordlist;
    solver_recursive s_solver (s_dictionary); 
    s_solver.solve (s_board, s_wordlist);

    TIMEIT (
        for (auto i = 0; i < 10; ++i) {
            s_solver.solve (s_board, s_wordlist);
            s_wordlist.clear ();
        }
    );


    return 0;
}
catch (const std::exception& ex) {
    std::cout << ex.what () << "\n";
    return -1;
}