#pragma once

#include <deque>
#include <vector>
#include "dictionary.hpp"
#include "board.hpp"

namespace ultimate_boggle {

    struct solver_dfs {        
        solver_dfs (dictionary& s_dictionary)
        :   m_dictionary (s_dictionary)
        {}

        void solve (const board& s_board, std::vector<const char*>&);

    private:        
        dictionary& m_dictionary;        
    };


}
