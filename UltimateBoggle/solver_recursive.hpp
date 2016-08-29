#pragma once

#include "dictionary.hpp"
#include "board.hpp"

#include <tuple>
#include <vector>
#include <deque>

namespace ultimate_boggle {

    struct solver_recursive {
        typedef std::int16_t coord_type;
        typedef std::unique_ptr<std::uint64_t []> checkbox_type;
        typedef std::vector<dictionary::state_type> state_list_type;

        solver_recursive (dictionary& s_dict);
        
        void solve (const board& s_board, std::vector<const char*>& s_list);
    private:

        void solve_step (const std::tuple<const board&, state_list_type&, checkbox_type&>&  s_context,
                         dictionary::state_type s_state,
                         coord_type s_posx,
                         coord_type s_posy)
                         const;

        dictionary& m_dictionary;
    };

}
