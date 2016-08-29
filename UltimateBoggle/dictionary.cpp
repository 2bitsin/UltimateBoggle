#include "dictionary.hpp"
#include "compile.hpp"
#include "utils.hpp"

#include <fstream>
#include <cassert>

static const auto st_slot_bitmask = (1u << 26u) - 1u;

ultimate_boggle::dictionary::dictionary (const std::string& s_file):
    m_data (nullptr),
    m_length (0u)
{
    const std::string s_file_cache = s_file + ".cache";
    std::ifstream s_ub32_dict (s_file_cache, std::ios::binary|std::ios::ate);
    if (!s_ub32_dict.good ()) {
        compile_from_text_file (s_file, s_file_cache);
        s_ub32_dict.clear ();
        s_ub32_dict.exceptions (s_ub32_dict.failbit);
        s_ub32_dict.open (s_file_cache, std::ios::binary|std::ios::ate);
        s_ub32_dict.exceptions (0);
    }

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
}

ultimate_boggle::dictionary::match_type
    ultimate_boggle::dictionary::next (state_type& s_state, char s_next) const 
{
    using namespace ultimate_boggle;

    const auto s_index = (std::uint8_t)s_next; 
    assert (s_index < 26u);

    //if (s_state == dictionary::state_type ()) {
    //    s_state = root ();
    //}    
    
    const auto* s_node = (const std::uint32_t*)&m_data [s_state];
    const auto s_mask = *s_node;

    if (!check_bit (s_mask, s_index)) {        
        return match_type_none;
    }
    
    const auto j = 1u + popcount (s_mask & ((1u << s_index) - 1u));
    s_state = s_node [j];
    s_node = (const std::uint32_t*)&m_data [s_state];
    return check_bit (*s_node, 31u)
         ? match_type_full
         : match_type_partial;

}

ultimate_boggle::dictionary::match_type 
    ultimate_boggle::dictionary::match (const char* s_key) const 
{
    auto s_state = root ();
    return match (s_key, s_state);
}

ultimate_boggle::dictionary::match_type
    ultimate_boggle::dictionary::match (const char* s_key, state_type& s_state) const
{    
    auto s_match = match_type_none;
    for (auto i = s_key; *i != '\0'; ++i) {
        s_match = next (s_state, *i - 'A');
        if (s_match == match_type_none) {
            return match_type_none;
        }
    }
    return s_match;
}

bool ultimate_boggle::dictionary::seen (state_type s_state) {
    auto* s_node = (std::uint32_t *)&m_data [s_state];
    auto s_flag = check_bit (s_node [0], 30u);
    s_node [0] |= (1u << 30u);
    return s_flag;
}

void ultimate_boggle::dictionary::unsee (state_type s_state) {
    auto* s_node = (std::uint32_t *)&m_data [s_state];
    s_node [0] &= (~(1u << 30u));
}

void ultimate_boggle::dictionary::unsee_branch (state_type s_branch) {
    using namespace ultimate_boggle;
    if (s_branch == dictionary::state_type ()) {
        return;
    }
    
    const auto* s_node = (const std::uint32_t*)&m_data [s_branch];
    const auto s_count = popcount (s_node [0] & st_slot_bitmask);
    unsee (s_branch);
        
    for (auto i = 0u; i < s_count; ++i) {            
         unsee_branch (s_node [1u + i]);
    }
}

void ultimate_boggle::dictionary::unsee_all () {
    unsee_branch (root ());
}

const char* ultimate_boggle::dictionary::cstring_at_node (state_type s_state) const {
    using namespace ultimate_boggle;
    const auto* s_node = (const std::uint32_t *)&m_data [s_state];
    if (check_bit (s_node [0], 31u)) {
        auto s_child_count = popcount (s_node [0] & st_slot_bitmask);        
        return (const char*)&s_node [1u + s_child_count];
    }
    return nullptr;
}

ultimate_boggle::dictionary::state_type 
    ultimate_boggle::dictionary::root () const 
{    
    return ((const std::uint32_t *)m_data.get ()) [1];
}
