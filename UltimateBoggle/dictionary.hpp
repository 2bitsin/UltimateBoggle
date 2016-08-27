#pragma once

#include <string>
#include <memory>

namespace ultimate_boggle {

    struct dictionary {
        typedef const std::uint8_t* state_type;

        enum match_type {
            match_type_none,
            match_type_full,
            match_type_partial
        };

        dictionary (const std::string& s_file);

        match_type next (state_type& s_state, char s_next) const;
        match_type find (const std::string& s_key) const;

        const std::string string_at_node (state_type);

    private:
        std::unique_ptr<std::uint8_t []> m_data;
        std::uint8_t* m_root;
        std::size_t m_length;
    };

}