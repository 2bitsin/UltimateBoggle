#include "solver_recursive.hpp"

using namespace ultimate_boggle;

solver_recursive::solver_recursive (dictionary& s_dict)
:   m_dictionary(s_dict) 
{

}

void solver_recursive::solve (const board& s_board, std::vector<const char*>& s_list) {
   
    auto s_totalbits = s_board.width ()*s_board.height ();
    s_totalbits = (s_totalbits + 63) >> 6u;
    auto s_visited = std::make_unique<std::uint64_t []> (s_totalbits);
    std::fill (s_visited.get (), s_visited.get () + s_totalbits, 0ull);
   
    state_list_type s_seen;
    s_seen.reserve (20000);
    auto&& s_context = std::tie (s_board, s_seen, s_visited);
    for (auto j = 0; j < s_board.height (); ++j) {
        for (auto i = 0; i < s_board.width (); ++i) {
            solve_step (s_context, m_dictionary.root (), i, j);
        }
    }

    s_list.reserve (s_seen.size () + 10);
    for (const auto& s_node : s_seen) {
        m_dictionary.unsee (s_node);
        s_list.emplace_back (m_dictionary.cstring_at_node (s_node));
    }   
}

inline void set_bit (solver_recursive::checkbox_type& s_cbox, std::uint32_t s_index) {
    const auto s_byte = s_index >> 6u;
    const auto s_nbit = s_index & 0x3Fu;
    s_cbox [s_byte] |= (1ull << s_nbit);
}

inline void clear_bit (solver_recursive::checkbox_type& s_cbox, std::uint32_t s_index) {
    const auto s_byte = s_index >> 6u;
    const auto s_nbit = s_index & 0x3Fu;
    s_cbox [s_byte] &= (~(1ull << s_nbit));
}

inline bool get_bit (const solver_recursive::checkbox_type& s_cbox, std::uint32_t s_index) {
    const auto s_byte = s_index >> 6u;
    const auto s_nbit = s_index & 0x3Fu;
    return s_cbox [s_byte] & (1ull << s_nbit) ? true : false;
}

inline bool is_valid_coords (
    const board& s_board,    
    const solver_recursive::checkbox_type& s_visited,
    solver_recursive::coord_type x, 
    solver_recursive::coord_type y) 
{
    return x >= 0 && y >= 0 && x < s_board.width () && y < s_board.height () 
        && !get_bit (s_visited, x + y*s_board.width ());
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

    auto s_new_state = s_state;
    switch (m_dictionary.next (s_new_state, s_board (s_posx, s_posy))) {
        case dictionary::match_type_none:
        {
            return;
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

            auto x = coord_type ();
            auto y = coord_type ();

            auto&& s_visited = std::get<checkbox_type&> (s_context);
            const auto s_index = s_posx + s_posy * s_board.width ();
            set_bit (s_visited, s_index);            
            x = s_posx - 1; y = s_posy - 1; if (is_valid_coords (s_board, s_visited, x, y)) solve_step (s_context, s_new_state, x, y);
            x = s_posx - 1; y = s_posy + 0; if (is_valid_coords (s_board, s_visited, x, y)) solve_step (s_context, s_new_state, x, y);
            x = s_posx - 1; y = s_posy + 1; if (is_valid_coords (s_board, s_visited, x, y)) solve_step (s_context, s_new_state, x, y);
            x = s_posx + 1; y = s_posy - 1; if (is_valid_coords (s_board, s_visited, x, y)) solve_step (s_context, s_new_state, x, y);
            x = s_posx + 1; y = s_posy + 0; if (is_valid_coords (s_board, s_visited, x, y)) solve_step (s_context, s_new_state, x, y);
            x = s_posx + 1; y = s_posy + 1; if (is_valid_coords (s_board, s_visited, x, y)) solve_step (s_context, s_new_state, x, y);
            x = s_posx + 0; y = s_posy - 1; if (is_valid_coords (s_board, s_visited, x, y)) solve_step (s_context, s_new_state, x, y);
            x = s_posx + 0; y = s_posy + 1; if (is_valid_coords (s_board, s_visited, x, y)) solve_step (s_context, s_new_state, x, y);
            clear_bit (s_visited, s_index);

        }
    }
}
