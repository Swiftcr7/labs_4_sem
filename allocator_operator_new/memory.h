//
// Created by HP on 13.08.2023.
//

#ifndef LABS_4_SEM_MEMORY_H
#define LABS_4_SEM_MEMORY_H
#include <iostream>
class memory{
public:
    virtual void* allocate(size_t target_size) const = 0;

    virtual void deallocate(void* result) const = 0;

    virtual ~memory(){

    }
};
#endif //LABS_4_SEM_MEMORY_H
