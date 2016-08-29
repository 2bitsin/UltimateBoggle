#pragma once

#include <memory>
#include <string>
#include <cassert>

namespace ultimate_boggle {


    struct board {
        typedef char value_type;

        board (const std::string& s_path);

        auto width  () const { return m_width;  }
        auto height () const { return m_height; }

        auto operator [] (std::size_t i) const {
            return m_data [i];
        }

        auto operator () (std::int32_t x, std::int32_t y) const {
            assert (x >= 0 && x < m_width && y >= 0 && y < m_height);
            return m_data [x + y*m_width];
        }

    private:
        std::unique_ptr<value_type []> m_data;
        std::int32_t m_width, m_height;
        
    };

}