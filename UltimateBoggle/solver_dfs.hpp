#pragma once

#include <forward_list>
#include "dictionary.hpp"
#include "board.hpp"

namespace ultimate_boggle {

    struct solver_dfs {        
        solver_dfs (dictionary& s_dictionary)
        :   m_dictionary (s_dictionary)
        {}

        void solve (const board& s_board, std::forward_list<const char*>&);

    private:        
        dictionary& m_dictionary;        
    };


}
