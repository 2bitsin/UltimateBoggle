#include "dictionary.hpp"
#include "compile.hpp"
#include "utils.hpp"

#include <iostream>
#include <algorithm>
#include <cassert>
#include <cctype>


int main () try {
    using namespace ultimate_boggle;    
    dictionary s_dictionary ("dict.txt");

}
catch (const std::exception& ex) {
    std::cout << ex.what () << "\n";
    return -1;
}