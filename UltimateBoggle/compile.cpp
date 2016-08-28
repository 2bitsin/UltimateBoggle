#include "compile.hpp"
#include "dictionary.hpp"
#include "trie.hpp"
#include "utils.hpp"

#include <iostream>
#include <iomanip>

#include <fstream>
#include <type_traits>
#include <string>
#include <algorithm>
#include <vector>
#include <cassert>


typedef ultimate_boggle::trie<char, 'A', 'Z'> trie_type;

struct dictionary_serializer {

    template <typename _Wtype>
    auto write (const _Wtype* s_value, std::size_t s_length) {
        m_node_stream.write (reinterpret_cast<const char *> (s_value),
            sizeof (*s_value) * s_length);
    }

    template <typename _Wtype, typename = decltype (std::begin (std::declval<_Wtype> ()))>
    auto write (const _Wtype& s_vector) {
        return write (std::data (s_vector), std::size (s_vector));
    }

    template <typename _Wtype, std::enable_if_t<std::is_pod<_Wtype>::value, int> = 0>
    auto write (const _Wtype& s_value) {
        return write (&s_value, 1u);
    }

    std::uint32_t write_recursive (const trie_type::node* s_node, std::string s_string = "") {
        using namespace ultimate_boggle;

        std::uint32_t s_mask = 0u;
        std::vector<std::uint32_t> s_node_offsets;
        s_node_offsets.reserve (32u);

        const auto s_children = std::size (s_node->children);
        for (auto i = 0u; i < s_children; ++i) {
            const auto& s_next = s_node->children [i];
            if (s_next == nullptr)
                continue;
            s_mask |= (1u << i);
            const auto s_offs = write_recursive (s_next.get (), 
                s_string + static_cast<char> ('A' + i));
            s_node_offsets.emplace_back (s_offs);
        }

        if (s_node->is_full_match) {
            s_mask |= 0x80000000;
        }

    #if 0        
        std::string s_padding (s_string.length (), ' ');
        std::cout << s_padding << "s : " << s_string <<  "\n";
        std::cout << s_padding << "m : " << to_base (s_mask, 2, 32) <<  "\n";
        for (auto i = 0u; i < s_node_offsets.size (); ++i) {
            std::cout << s_padding << i << " : " << to_base (s_node_offsets [i], 16, 8) <<  "\n";
        }
    #endif

        const auto s_offset = (std::uint32_t)m_node_stream.tellp ();
        write (s_mask);
        write (s_node_offsets);

        if (s_node->is_full_match) {            
            s_string.push_back ('\0');
            const auto s_len = ((s_string.length () + 3u) & ~std::size_t (3u));
            while (s_string.length () < s_len)
                s_string.push_back ('\0');
            write (s_string);
        }

        return s_offset;
    }

    void serialize_trie (const trie_type& s_trie) {
        write ("UB32", 4u);
        write (std::uint32_t ());
        const auto s_root_offset = write_recursive (s_trie.root ());
        m_node_stream.seekp (4u);
        write (s_root_offset);
    }

    dictionary_serializer (std::ostream& s_node_stream):
        m_node_stream (s_node_stream),
        m_node_index (0u),
        m_node_store (nullptr) 
    {}

private:
    std::ostream& m_node_stream;
    std::uint32_t m_node_index;
    std::unique_ptr<std::uint8_t []> m_node_store;
};


void ultimate_boggle::compile_from_text_file (
    const std::string& s_source, 
    const std::string& s_dest) 
{

    trie_type s_trie;
    std::ifstream s_dict_text (s_source);
    s_dict_text.exceptions (s_dict_text.failbit);
    s_dict_text.exceptions (0);
    std::string s_word;
    
    while (std::getline (s_dict_text, s_word)) {
        s_trie.insert (s_word);
    }

    std::ofstream s_dict_binary (s_dest, std::ios::binary);
    s_dict_binary.exceptions (s_dict_binary.failbit);
    dictionary_serializer s_serializer (s_dict_binary);
    s_serializer.serialize_trie (s_trie);
}
