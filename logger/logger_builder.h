
#ifndef LABS_4_SEM_LOGGER_BUILDER_H
#define LABS_4_SEM_LOGGER_BUILDER_H
#include <iostream>
#include "logger_concrete.h"

class logger_builder{
public:
    virtual ~logger_builder() noexcept = default;

    virtual logger_builder* add_stream(std::string const &, logger::severity)=0;

    virtual logger* construct() const = 0;

    virtual logger_builder* add_in_file(const std::string& way) = 0;
};
#endif //LABS_4_SEM_LOGGER_BUILDER_H
