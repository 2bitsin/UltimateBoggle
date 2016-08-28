#pragma once

#include <string>
#include <memory>
#include <forward_list>

namespace ultimate_boggle {

    struct dictionary {
        typedef std::uint32_t state_type;

        enum match_type {
            match_type_none,
            match_type_full,
            match_type_partial
        };

        dictionary (const std::string& s_file);
        
        match_type next (state_type& s_state, char s_next) const;
        match_type match (const std::string& s_key) const;
        match_type match (const std::string& s_key, state_type &s_state) const;
       
        bool seen (state_type s_state);
        void unsee (state_type s_state);
        void unsee_branch (state_type s_branch);
        void unsee_all ();
        
        const char* cstring_at_node (state_type) const;
        state_type root () const;

    private:
        std::unique_ptr<std::uint8_t []> m_data;
        std::size_t m_length;           
    };

}