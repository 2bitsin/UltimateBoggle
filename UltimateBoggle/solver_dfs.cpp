#include "solver_dfs.hpp"
#include <deque>

void ultimate_boggle::solver_dfs::solve (const board& s_board, std::forward_list<const char*>& s_list) {
    typedef int coord_type;
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
        board_state_type (coord_type s_posx, 
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

    static const auto validate_coords = [] (const board& s_board, const board_state_type* s_state, coord_type x, coord_type y) {
        if (x < 0 || y < 0 
         || x >= s_board.width () 
         || y >= s_board.height ())
            return false;
        while (s_state) {
            if (s_state->m_posx == x ||
                s_state->m_posy == y)
                return false;
            s_state = s_state->m_parent;
        }
        return true;
    };

    std::forward_list<board_state_type*> s_fringe;
    std::forward_list<dictionary::state_type> s_seenlist;
    std::deque<board_state_type> s_all_states;

    for (coord_type j = 0; j < s_board.height (); ++j) {
        for (coord_type i = 0; i < s_board.width (); ++i) {
            s_all_states.emplace_back (i, j, 0u);
            s_fringe.emplace_front (&s_all_states.back ());
        }
    }

    while (!s_fringe.empty ()) {
        auto* s_current = s_fringe.front ();
        s_fringe.pop_front ();
        const auto s_next_char = s_board (s_current->m_posx, s_current->m_posy);
        auto s_result = m_dictionary.next (s_current->m_node, s_next_char);
        if (s_result == dictionary::match_type_none) {
            continue;
        }

        if (s_result == dictionary::match_type_full) {
            if (!m_dictionary.seen (s_current->m_node)) {
                s_seenlist.push_front (s_current->m_node);
                s_list.emplace_front (m_dictionary.cstring_at_node (s_current->m_node));
            }
        }

        for (const auto& s_delta : s_deltas) {
            auto s_newx = s_current->m_posx + s_delta [0];
            auto s_newy = s_current->m_posy + s_delta [1];

            
        }
    }

}
