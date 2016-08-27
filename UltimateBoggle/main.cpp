#include "dictionary.hpp"
#include "compile.hpp"

#include <iostream>

int main () try {
    //ultimate_boggle::compile_from_text_file ("dict.txt", "dict.ub32");
    ultimate_boggle::dictionary s_dictionary ("dict.ub32");

    auto s_result = s_dictionary.find ("FEMININENESSES");

    return 0;
}
catch (const std::exception& ex) {
    std::cout << ex.what () << "\n";
    return -1;
}