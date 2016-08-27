#pragma once

#include <memory>
#include <cctype>
#include <string>
#include <cassert>

namespace ultimate_boggle {

    template <typename _Ctype, _Ctype _Start, _Ctype _End>
    struct trie {
        static const auto value_first = _Start;
        static const auto value_last  = _End;
        static const auto value_range = _End - _Start + 1;

        typedef _Ctype value_type;
        enum match_type {
            match_type_none,
            match_type_full,
            match_type_partial
        };

        struct node {
            node ():
                children{nullptr},
                is_full_match (false)
            {}

            std::unique_ptr<node> children [value_range];
            bool is_full_match;
        };

        void insert (const std::basic_string<value_type>& s_word) {
            auto* s_curr = &m_root;
            for (auto s_char : s_word) {
                assert (s_char >= value_first && s_char <= value_last);
                const auto s_index = (s_char - value_first);
                std::unique_ptr<node>& s_slot = s_curr->children [s_index];
                if (!s_slot) {
                    s_slot = std::make_unique<node> ();
                }
                s_curr = s_slot.get ();
            }
            s_curr->is_full_match = true;
        }

        match_type find (const std::basic_string<value_type>& s_word) const {
            node* s_curr = nullptr;
            auto s_match = match_type_none;
            for (const auto& s_char : s_word) {
                s_match = next (s_curr, s_char);
                if (s_match == match_type_none) {
                    return s_match;
                }
            }
            return s_match;
        }

        match_type next (const node* (&s_node), value_type s_char) const {
            assert (s_char >= value_first && s_char <= value_last);

            if (s_node == nullptr) {
                s_node = &m_root;
            }

            s_node = s_node->children [s_char - value_first];
            if (s_node != nullptr) {
                return s_node->is_full_match
                    ? match_type_full
                    : match_type_partial;
            }
            return match_type_none;
        }

        const node* root () const {
            return &m_root;
        }

        node* root () {
            return &m_root;
        }

    private:
        node m_root;
    };
}