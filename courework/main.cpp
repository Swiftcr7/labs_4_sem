#include <iostream>
#include "database.h"
int main() {
    logger_builder* builder = new logger_builder_concrete();
    logger* mylogger = builder->add_stream("console", logger::severity::warning)->add_stream("C:\\Users\\HP\\CLionProjects\\labs_4_sem\\courework\\debug.txt", logger::severity::debug)->construct();
    auto* data = new database(mylogger);
    try{
        data->dialogue();
    }catch (const std::logic_error& er){
        std::cout<<er.what()<<std::endl;
    }
    delete data;
    delete mylogger;
    delete builder;
    return 0;
}
