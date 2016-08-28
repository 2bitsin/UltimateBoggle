#include "board.hpp"
#include <fstream>

ultimate_boggle::board::board (const std::string& s_path) {
    std::int32_t s_width, s_height;
    std::basic_ifstream<value_type> s_board_stream (s_path);

    s_board_stream.exceptions (s_board_stream.failbit);
    s_board_stream.exceptions (0);

    s_board_stream >> s_width >> s_height;

    auto s_buff = std::make_unique<value_type []> (s_width*s_height);

    std::int32_t s_row = 0u;
    std::basic_string<value_type> s_line;
    std::getline (s_board_stream, s_line);
    while (std::getline (s_board_stream, s_line)) {
        s_line = s_line.substr (0, s_width);
        assert (s_line.size () >= s_width);
        for (const auto& s_char : s_line) {
            assert (s_char >= 'A' && s_char <= 'Z');
        }
        std::memcpy (s_buff.get () + s_row * s_width, s_line.data (), s_width);
        ++s_row;
    }
    assert (s_row == s_height);

    m_data = std::move (s_buff);
    m_width = s_width;
    m_height = s_height;
}
