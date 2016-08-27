#include "dictionary.hpp"
#include <fstream>
#include <cassert>

ultimate_boggle::dictionary::dictionary (const std::string& s_file):
    m_data (nullptr),
    m_root (nullptr),
    m_length (0u)
{
    std::ifstream s_ub32_dict (s_file, std::ios::binary|std::ios::ate);
    s_ub32_dict.exceptions (s_ub32_dict.failbit);    
    std::size_t s_length = s_ub32_dict.tellg ();
    s_ub32_dict.seekg (0u);
    auto s_data = std::make_unique<std::uint8_t []> (s_length);
    s_ub32_dict.read (reinterpret_cast<char *> (s_data.get ()), s_length);    

    if (std::memcmp (s_data.get (), "UB32", 4u) != 0) {
        throw std::runtime_error ("Bad dictionary file.");
    }

    std::uint32_t s_root_offset = 0u;
    std::memcpy (&s_root_offset, s_data.get () + 4u, sizeof (s_root_offset));

    if (s_root_offset >= s_length) {
        throw std::runtime_error ("Corrupt dictionary file.");
    }

    m_data = std::move (s_data);
    m_length = s_length;
    m_root = m_data.get () + s_root_offset;   
}


inline bool check_bit (std::uint32_t s_mask, std::uint32_t s_bit) {
    return !!(s_mask & (1u << s_bit));
}

ultimate_boggle::dictionary::match_type
    ultimate_boggle::dictionary::next (state_type& s_state, char s_next) const 
{
    if (s_state == nullptr) {
        s_state = m_root;
    }

    std::uint8_t s_index = s_next - 'A';
    assert (s_index < 26u);

    const auto* s_node = (const std::uint32_t*)s_state;

    const auto s_mask = s_node [0];
    if (check_bit (s_mask, s_index)) {
        auto i = 0u, j = 0u;
        while (i < s_index) {
            if (check_bit (s_mask, i)) ++j;
            ++i;
        }
        s_state = m_data.get () + s_node [1u + j];
        return check_bit (s_node [0], 31u) 
             ? match_type_full 
             : match_type_partial;
    }

    return match_type_none;
}

ultimate_boggle::dictionary::match_type 
    ultimate_boggle::dictionary::find (const std::string& s_key) const 
{
    const std::uint8_t* s_state = nullptr;
    auto s_match = match_type_none;
    for (const auto& s_char : s_key) {
        auto s_match = next (s_state, s_char);
        if (s_match == match_type_none) {
            return match_type_none;
        }
    }
    return s_match;
}

const std::string ultimate_boggle::dictionary::string_at_node (state_type s_state) {
    const auto* s_node = (const std::uint32_t*)s_state;
    if (s_node [0] & 0x80000000) {
        auto s_child_count = __popcnt (s_node [0]);
        const auto* s_string = s_state + sizeof (std::uint32_t) * (1u + s_child_count);
        const auto s_length = *s_string;
        return std::string (s_string + 1u, s_string + s_length);
    }
    throw std::runtime_error ("No string bound to this node");
}
