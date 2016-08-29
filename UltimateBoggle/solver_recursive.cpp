#include "solver_recursive.hpp"

using namespace ultimate_boggle;

solver_recursive::solver_recursive (dictionary& s_dict)
:   m_dictionary(s_dict) 
{

}

void solver_recursive::solve (const board& s_board, std::vector<const char*>& s_list) {
   
    std::vector<bool> s_visited (s_board.width ()*s_board.height (), false);
    std::deque<dictionary::state_type> s_seen;
    for (auto j = 0; j < s_board.height (); ++j) {
        for (auto i = 0; i < s_board.width (); ++i) {
            solve_step (std::tie (s_board, s_seen, s_visited), 0u, i, j);
        }
    }

    for (const auto& s_node : s_seen) {
        m_dictionary.unsee (s_node);
        s_list.emplace_back (m_dictionary.cstring_at_node (s_node));
    }   
}

inline bool is_valid_coords (
    const board& s_board,
    const solver_recursive::checkbox_type& s_visited,
    solver_recursive::coord_type x, 
    solver_recursive::coord_type y) 
{
    return x >= 0 && y >= 0
        && x < s_board.width ()
        && y < s_board.height ()
        && !s_visited [x + y * s_board.width ()];
}

void ultimate_boggle::solver_recursive::solve_step (
    const std::tuple<const board&, state_list_type&, checkbox_type&>& s_context,
    dictionary::state_type s_state,   
    coord_type s_posx, 
    coord_type s_posy) 
    const
{
    auto&& s_board = std::get<const board&> (s_context);
    auto&& s_seen = std::get<state_list_type&> (s_context);
    auto&& s_visited = std::get<checkbox_type&> (s_context);

    static const coord_type s_deltas [8][2] = {
        {-1, -1},
        {-1, +0},
        {-1, +1},
        {+1, -1},
        {+1, +0},
        {+1, +1},
        {+0, -1},
        {+0, +1}
    };

    auto s_new_state = s_state;
    const auto s_result = m_dictionary.next (s_new_state, s_board (s_posx, s_posy));
    auto&& s_bit = s_visited [s_posx + s_posy * s_board.width ()];

    s_bit = true;
    switch (s_result) {
        case dictionary::match_type_none:
        {
            break;
        }

        case dictionary::match_type_full:
        {
            const auto* s_string = m_dictionary.cstring_at_node (s_new_state);
            if (s_string [1] != 0 && s_string [2] != 0) {
                if (!m_dictionary.seen (s_new_state)) {
                    s_seen.emplace_back (s_new_state);
                }
            }
        }
            
        case dictionary::match_type_partial:
        {

            auto x = coord_type (), y = coord_type ();
            
            x = s_posx - 1; y = s_posy - 1; if (is_valid_coords (s_board, s_visited, x, y)) solve_step (s_context, s_new_state, x, y);
            x = s_posx - 1; y = s_posy + 0; if (is_valid_coords (s_board, s_visited, x, y)) solve_step (s_context, s_new_state, x, y);
            x = s_posx - 1; y = s_posy + 1; if (is_valid_coords (s_board, s_visited, x, y)) solve_step (s_context, s_new_state, x, y);
            x = s_posx + 1; y = s_posy - 1; if (is_valid_coords (s_board, s_visited, x, y)) solve_step (s_context, s_new_state, x, y);
            x = s_posx + 1; y = s_posy + 0; if (is_valid_coords (s_board, s_visited, x, y)) solve_step (s_context, s_new_state, x, y);
            x = s_posx + 1; y = s_posy + 1; if (is_valid_coords (s_board, s_visited, x, y)) solve_step (s_context, s_new_state, x, y);
            x = s_posx + 0; y = s_posy - 1; if (is_valid_coords (s_board, s_visited, x, y)) solve_step (s_context, s_new_state, x, y);
            x = s_posx + 0; y = s_posy + 1; if (is_valid_coords (s_board, s_visited, x, y)) solve_step (s_context, s_new_state, x, y);
        }
    }
    s_bit = false;
}
