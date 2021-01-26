#pragma once
#include <locale>
#include <iomanip>
#include <string>
#include <sstream>
#include <iostream>

#if defined (_DEBUG) || defined (DEBUG)
#include "ld/LeakDetector.hpp"
#endif

template<class T>
std::string RecursiveCommas(T n) {

    if (n < 1000)
        return std::to_string(n);
    else
        return RecursiveCommas(n / 1000) + "," + std::to_string(n % 1000);

}