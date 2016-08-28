#pragma once

#include <memory>
#include <string>

namespace ultimate_boggle {

    struct board {
        board (const std::string& s_path);

    private:
        std::unique_ptr<std::uint8_t []> m_data;
        std::uint32_t m_width, m_height;
        
    };

}