#include "borad.hpp"
#include <fstream>

ultimate_boggle::board::board (const std::string& s_path) {
    std::uint32_t s_width, s_height;
    std::ifstream s_board_stream (s_path);

    s_board_stream.exceptions (s_board_stream.failbit);
    s_board_stream.exceptions (0);

    s_board_stream >> s_width >> s_height;

    auto s_buff = std::make_unique<std::uint8_t []> (s_width*s_height);

    std::uint32_t s_row = 0u;
    std::string s_line;

    while (std::getline (s_board_stream, s_line)) {
        std::memcpy (s_buff.get () + s_row * s_width, s_line.data (), s_width);
    }

    m_data = std::move (s_buff);
    m_width = s_width;
    m_height = s_height;
}
