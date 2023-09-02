#include <iostream>
#include "allocator_operator_new.h"
int main() {
    logger_builder* builder = new logger_builder_concrete();
    logger* mylogger = builder->add_stream("console", logger::severity::debug)->add_stream("C:\\Users\\HP\\CLionProjects\\labs_4_sem\\allocator_buddies\\logs.txt", logger::severity::debug)->construct();
    memory* alloc = new allocator( mylogger);
    auto* v = alloc->allocate(24);
    alloc->deallocate(v);

}
