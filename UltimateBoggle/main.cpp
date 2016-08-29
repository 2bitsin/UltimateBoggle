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

#define __TEST__ 0

int main () try {
    using namespace ultimate_boggle;
    dictionary s_dictionary ("dict.txt");    
    board s_board ("board.txt");    


#if __TEST__

    std::vector<const char*> s_wordlist0;
    std::vector<const char*> s_wordlist1;

    solver_dfs       s_solver_dfs (s_dictionary);
    solver_recursive s_solver_recursive (s_dictionary);

    TIMEIT (s_solver_dfs.solve (s_board, s_wordlist0));
    TIMEIT (s_solver_recursive.solve (s_board, s_wordlist1));

    if (s_wordlist0.size () != s_wordlist1.size ()) {
        throw std::runtime_error ("Failed : s_wordlist0.size () != s_wordlist1.size ()");
    }

    std::sort (s_wordlist0.begin (), s_wordlist0.end (), [] (auto a, auto b) {
        return std::string(a) < std::string(b);
    });
    std::sort (s_wordlist1.begin (), s_wordlist1.end (), [] (auto a, auto b) {
        return std::string (a) < std::string (b);
    });

    for (auto i = 0; i < s_wordlist0.size (); ++i) {
        std::cout << s_wordlist0 [i] << "\n";

        if (s_dictionary.match (s_wordlist0 [i]) != dictionary::match_type_full) {
            throw std::runtime_error ("Failed : s_dictionary.match (s_wordlist0 [i]) != dictionary::match_type_full");
        }

        if (s_wordlist0 [i] != s_wordlist1 [i]) {
            throw std::runtime_error ("Failed : s_wordlist0 [i] != s_wordlist1 [i]");
        }
    }
#endif

#if !__TEST__
    std::vector<const char*> s_wordlist;
    solver_recursive s_solver (s_dictionary); 
    s_solver.solve (s_board, s_wordlist);

    TIMEIT (
        for (auto i = 0; i < 10; ++i) {
            s_solver.solve (s_board, s_wordlist);
            s_wordlist.clear ();
        }
    );
#endif

    /*
    auto s_wlist = glob ("dict.txt");
    volatile dictionary::match_type s_result;
    for (auto i = 0; i < 1000; ++i)
    for (const auto& s_word : s_wlist) {
         s_result = s_dictionary.match (s_word.c_str ());
    }
    */


    return 0;
}
catch (const std::exception& ex) {
    std::cout << ex.what () << "\n";
    return -1;
}