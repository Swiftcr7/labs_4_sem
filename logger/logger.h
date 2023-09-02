#ifndef LABS_4_SEM_LOGGER_H
#define LABS_4_SEM_LOGGER_H
#include <iostream>
class logger{
public:
    enum class severity{
        trace,
        debug,
        information,
        warning,
        error,
        critical
    };
    virtual ~logger(){

    };
    virtual logger const* log(const std::string& target, severity) const = 0;
};

#endif //LABS_4_SEM_LOGGER_H
