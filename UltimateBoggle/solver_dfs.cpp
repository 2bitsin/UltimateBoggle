#include "solver_dfs.hpp"
#include <deque>

void ultimate_boggle::solver_dfs::solve (const board& s_board, std::vector<const char*>& s_list) {
    typedef std::int16_t coord_type;
    static const int s_deltas [8u] [2u] = {
        {-1, -1},
        {-1, +0},
        {-1, +1},
        {+1, -1},
        {+1, +0},
        {+1, +1},
        {+0, -1},
        {+0, +1}
    };

    struct board_state_type {
        board_state_type (
            coord_type s_posx, 
            coord_type s_posy,
            dictionary::state_type s_node,
            board_state_type* s_parent)
        :   m_posx (s_posx), 
            m_posy (s_posy),
            m_node (s_node),
            m_parent (s_parent)
        {}

        coord_type m_posx, m_posy;
        dictionary::state_type m_node;
        board_state_type* m_parent;
    };

    static const auto validate_coords = 
    [] (const board& s_board, 
        const board_state_type* s_state, 
        coord_type x, 
        coord_type y) 
    {
        if (x < 0 || y < 0 || x >= s_board.width () || y >= s_board.height ()) {
            return false;
        }

        while (s_state) {
            if (s_state->m_posx == x && s_state->m_posy == y)
                return false;
            s_state = s_state->m_parent;
        }
        return true;
    };

    std::deque<board_state_type*> s_fringe;
    std::deque<board_state_type> s_all_states;
    std::deque<dictionary::state_type> s_seenlist;

    for (coord_type j = 0; j < s_board.height (); ++j) {
        for (coord_type i = 0; i < s_board.width (); ++i) {
            auto s_state = dictionary::state_type ();
            auto s_reslut = m_dictionary.next (s_state, s_board (i, j));
            if (s_reslut != dictionary::match_type_none) {
                s_all_states.emplace_back (i, j, s_state, nullptr);
                s_fringe.emplace_front (&s_all_states.back ());
            }
        }
    }

    while (!s_fringe.empty ()) {
        auto* s_current = s_fringe.back ();
        s_fringe.pop_back ();

        for (const auto& s_delta : s_deltas) {
            auto s_newx = s_current->m_posx + s_delta [0];
            auto s_newy = s_current->m_posy + s_delta [1];

            if (!validate_coords (s_board, s_current, s_newx, s_newy)) {
                continue;
            }

            auto s_new_state = s_current->m_node;
            const auto s_result = m_dictionary.next (s_new_state, s_board (s_newx, s_newy));

            switch (s_result) {

                case dictionary::match_type_none:
                    continue;
                
                case dictionary::match_type_full:
                {
                    const auto* s_cstring = m_dictionary.cstring_at_node (s_new_state);

                    if (s_cstring [1] != '\0' && s_cstring [2] != '\0') {
                        if (!m_dictionary.seen (s_new_state)) {
                            s_seenlist.emplace_back (s_new_state);
                        }
                    }
                }

                case dictionary::match_type_partial:
                {
                    s_all_states.emplace_back (s_newx, s_newy, s_new_state, s_current);
                    s_fringe.emplace_back (&s_all_states.back ());
                }
            }
        }
    }
    s_list.reserve (s_seenlist.size ());
    for (auto& s_node : s_seenlist) {
        m_dictionary.unsee (s_node);
        s_list.push_back (m_dictionary.cstring_at_node (s_node));        
    }

}
