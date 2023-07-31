#ifndef LABS_4_SEM_MAKE_STRING_H
#define LABS_4_SEM_MAKE_STRING_H
#include <iostream>
#include <sstream>
template <typename T>
std::string make_string(T a){
    std::stringstream stream;
    stream << a;
    return stream.str();
}
#endif //LABS_4_SEM_MAKE_STRING_H
