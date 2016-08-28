#pragma once

#include <string>
#include <fstream>
#include <list>
#include <algorithm>
#include <cstdint>
#include <cctype>
#include <chrono>

namespace ultimate_boggle {
    inline std::string to_upper (std::string value) {
        std::transform (value.begin (), value.end (), value.begin (), [] (auto c) {
            return std::toupper (c);
        });
        return value;
    }


    inline std::string to_base (std::uint64_t value, std::uint8_t base, std::uint8_t mindigits) {
        std::string s_output;
        static const char s_alphabet [] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz#*";

        base = std::min<std::uint8_t> (64u, base);

        do {
            s_output = s_alphabet[value % base] + s_output;
            value /= base;
            --mindigits;
        }
        while (value > 0 || mindigits > 0);

        return s_output;
    }


    inline auto glob (const std::string& s_path) {
        std::list<std::string> s_list;
        std::ifstream s_stream (s_path);
        //s_stream.exceptions (s_stream.failbit);
        //s_stream.exceptions (0);
        std::string s_line;
        while (std::getline (s_stream, s_line)) {
            s_list.emplace_back (s_line);
        }
        return s_list;
    }

//#define NO_INTRINSICS

#if !defined(NO_INTRINSICS) && defined (_MSC_VER)

    inline auto popcount (std::uint16_t value) { return __popcnt16 (value); }
    inline auto popcount (std::uint32_t value) { return __popcnt (value); }
    inline auto popcount (std::uint64_t value) { return __popcnt64 (value); }

#elif !defined(NO_INTRINSICS) && (defined (__GNUC__) || defined (__clang__))

    inline auto popcount (std::uint16_t value) { return __builtin_popcount (value); }
    inline auto popcount (std::uint32_t value) { return __builtin_popcountl (value); }
    inline auto popcount (std::uint64_t value) { return __builtin_popcountll (value); }

#else

    template <typename T>
    inline std::uint8_t popcount (T value) {
        std::uint8_t s_pcnt = 0u;
        while (value) {
            s_pcnt += (value&1);
            value >>= 1u;
        }
        return s_pcnt;
    }

#endif

    template <typename _Ttype = std::chrono::milliseconds, typename _Function>
    auto time (_Function&& s_func) {
        auto t0 = std::chrono::high_resolution_clock::now ();
        s_func ();
        auto t1 = std::chrono::high_resolution_clock::now ();
        return std::chrono::duration_cast<_Ttype>(t1 - t0).count ();
    }

    #define TIMEIT(...) std::cout << #__VA_ARGS__ << "\nTook : " << time([&] () {__VA_ARGS__;}) << " ms."

}