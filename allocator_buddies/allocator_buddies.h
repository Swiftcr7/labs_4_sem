#ifndef LABS_4_SEM_ALLOCATOR_BUDDIES_H
#define LABS_4_SEM_ALLOCATOR_BUDDIES_H
#include <iostream>
#include "../../scc//allocator2.h"
#include <cmath>
class allocator_buddies final : public memory{
private:
    void* _global_memory;
private:
    static bool get_employment(void* point) {
        if ((static_cast<int>(*get_size_block(point)) & 1) == 0){
            return true;
        }else{
            return false;
        }
    }

    void* get_buddies(void* current_block) const
    {
        size_t degree = static_cast<int>(*get_size_block(current_block) >> 1);

        if (degree == *get_size())
        {
            return nullptr;
        }

        auto * const start_allocated_memory = get_pointer_first() + 1;
        size_t size = 1 << degree;

        size_t relative_address = reinterpret_cast<unsigned char*>(current_block) - reinterpret_cast<unsigned char*>(start_allocated_memory);
        size_t relative_address_buddies = relative_address ^ size;

        return reinterpret_cast<void*>(reinterpret_cast<unsigned char*>(start_allocated_memory) + relative_address_buddies);
    }


    static void change_employment(void* point){
        if (get_employment(point)){
            *reinterpret_cast<unsigned char*>(point) = static_cast<unsigned char>(static_cast<int>(*reinterpret_cast<unsigned char*>(point)) | 1);
        }else{
            *reinterpret_cast<unsigned char*>(point) = static_cast<unsigned char>((static_cast<int>(*reinterpret_cast<unsigned char*>(point)) >> 1) << 1);
        }
    }

    int* get_size() const{
        return reinterpret_cast<int*>(_global_memory);
    }

    logger** get_logger() const{
        return reinterpret_cast<logger**>(reinterpret_cast<int*>(_global_memory)+1);
    }

    memory** get_allocator() const{
        return reinterpret_cast<memory**>(reinterpret_cast<logger*>(reinterpret_cast<int*>(_global_memory) + 1) + 1);
    }

    void** get_pointer_first() const{
        return reinterpret_cast<void**>(reinterpret_cast<memory**>(reinterpret_cast<logger*>(reinterpret_cast<int*>(_global_memory) + 1) + 1)+1);
    }

    static void** get_pointer_to_next_block(void* block) {
        return reinterpret_cast<void**>(reinterpret_cast<unsigned char*>(block) + 1);
    }

    static void** get_pointer_to_previous(void* block) {
        return reinterpret_cast<void**>(reinterpret_cast<void**>(reinterpret_cast<unsigned char*>(block)+1)+1);
    }

    static unsigned char* get_size_block(void* pointer){
        return reinterpret_cast<unsigned char*>(pointer);
    }


public:
    explicit allocator_buddies(size_t size, logger* log = nullptr, memory* allocator = nullptr){
        if (log != nullptr){
            log->log("---------The constructor has started working-----", logger::severity::debug);
        }
        int degree = static_cast<int>(std::ceil(log2(size)));
        if (allocator == nullptr){
            _global_memory = ::operator new((1<<degree) + sizeof(int) + sizeof(void*) + sizeof(logger*) + sizeof(memory*));
        }else{
            _global_memory = allocator->allocate((1<<degree)  + sizeof(int) + sizeof(void*) + sizeof(logger*) + sizeof(memory*));
        }
        if (log != nullptr){
            log->log("Memory: " + std::to_string(1<<degree), logger::severity::debug);
        }
        *reinterpret_cast<int*>(_global_memory) = degree;
        *reinterpret_cast<logger**>(reinterpret_cast<int*>(_global_memory)+1) = log;
        *reinterpret_cast<memory**>(reinterpret_cast<logger**>(reinterpret_cast<int*>(_global_memory) + 1) + 1) = allocator;
        auto* block = reinterpret_cast<void**>(reinterpret_cast<memory**>(reinterpret_cast<logger**>(reinterpret_cast<int*>(_global_memory) + 1) + 1) + 1) + 1;
        *reinterpret_cast<void**>(reinterpret_cast<memory**>(reinterpret_cast<logger**>(reinterpret_cast<int*>(_global_memory) + 1) + 1) + 1) = block;
        *reinterpret_cast<unsigned char*>(block) = degree*2;
        *reinterpret_cast<void**>(reinterpret_cast<unsigned char*>(block)+1) = nullptr;
        *reinterpret_cast<void**>(reinterpret_cast<void**>(reinterpret_cast<unsigned char*>(block)+1)+1) = nullptr;
        if (log != nullptr){
            log->log("---------The constructor has end working---------", logger::severity::debug);
        }
    }
public:
    void* allocate(size_t target_size) const override{
        logger* log = *get_logger();
        if (log != nullptr){
            log->log("---------The allocate has started working--------", logger::severity::debug);
        }
        size_t required_size = target_size + 2*sizeof(void*) + sizeof(unsigned char);
        auto* current_block = *get_pointer_first();
        while (current_block != nullptr){
            int degree = static_cast<int>((*get_size_block(current_block)) >> 1);
            if ((1 << degree) >= required_size){
                auto* current_block_previous = *get_pointer_to_previous(current_block);
                auto* current_block_next = *get_pointer_to_next_block(current_block);
                while (((1 << degree) >> 1) > required_size){
                    degree--;
                    void* baddie = reinterpret_cast<void*>(reinterpret_cast<unsigned char*>(current_block) + (1 << degree));
                    if (current_block_next != nullptr){
                        *get_pointer_to_previous(current_block_next) = baddie;
                    }
                    *get_size_block(baddie) = 2*degree;
                    *get_pointer_to_previous(baddie) = reinterpret_cast<void*>(current_block);
                    *get_pointer_to_next_block(baddie) = reinterpret_cast<void*>(current_block_next);
                    *get_size_block(current_block) = degree*2;
                    *get_pointer_to_next_block(current_block) = baddie;
                    current_block_next = baddie;
                }
                change_employment(current_block);
                if (current_block_previous == nullptr){
                    *get_pointer_first() = current_block_next;
                }else{
                    *get_pointer_to_next_block(current_block_previous) = current_block_next;
                }
                if (current_block_next != nullptr){
                    *get_pointer_to_previous(current_block_next) = current_block_previous;
                }
                if (log != nullptr){
                    log->log("Memory allocation has been successfully completed", logger::severity::debug);
                    log->log("---------The allocate has finished working--------", logger::severity::debug);
                }
                return get_size_block(current_block) + 1;

            }
            current_block = *get_pointer_to_next_block(current_block);
        }
        if (log != nullptr){
            log->log("It is impossible to allocate so much memory", logger::severity::debug);
            log->log("---------The allocate has finished working--------", logger::severity::debug);
        }
        return nullptr;
    }

    void deallocate(void* result) const override{
        logger* log = *get_logger();
        if (log != nullptr){
            log->log("---------The deallocate has started working--------", logger::severity::debug);
        }
        if (result == nullptr){
            return;
        }

        auto* current_block = reinterpret_cast<void*>(reinterpret_cast<unsigned char*>(result) - 1);
        size_t size = static_cast<int>((*get_size_block(current_block)) >> 1);
        std::string result_log = "[ ";
        for (int i = 0; i < 1<<size; i++) {
            unsigned char a = *(reinterpret_cast<unsigned char *>(result) + i);
            result_log += std::to_string(static_cast<unsigned short>(a));
            result_log += " ";
        }
        std::string result_msg = "Block to deallocate: " + result_log + "]";
        if (log != nullptr) {
            log->log(result_msg, logger::severity::debug);
        }
        void* previous_block = nullptr;
        void* next_block = *get_pointer_first();
        while (next_block != nullptr && reinterpret_cast<unsigned char*>(current_block) - reinterpret_cast<unsigned char*>(next_block) > 0){
            previous_block = next_block;
            next_block = *get_pointer_to_next_block(next_block);
        }
        change_employment(current_block);
        *get_pointer_to_next_block(current_block) = next_block;
        *get_pointer_to_previous(current_block) = previous_block;
        if (previous_block == nullptr){
            *get_pointer_first() = current_block;
        }else{
            *get_pointer_to_next_block(previous_block) = current_block;
        }
        if (next_block != nullptr){
            *get_pointer_to_previous(next_block) = current_block;
        }
        void* buddie = get_buddies(current_block);
        while (buddie != nullptr && get_employment(buddie) && static_cast<int>((*get_size_block(current_block)) >> 1) == static_cast<int>((*get_size_block(buddie)) >> 1)){
            if (log != nullptr){
                log->log("----------Merging----------", logger::severity::debug);
            }
            if (reinterpret_cast<unsigned char*>(buddie) - reinterpret_cast<unsigned char*>(current_block) < 0){
                auto* tmp = current_block;
                current_block = buddie;
                buddie = tmp;
            }
            *get_pointer_to_next_block(current_block) = *get_pointer_to_next_block(buddie);
            *get_size_block(current_block) = (static_cast<int>((*get_size_block(current_block)) >> 1) + 1) * 2;
            if (*get_pointer_to_next_block(buddie) != nullptr){
                *get_pointer_to_previous(*get_pointer_to_next_block(buddie)) = current_block;
            }
            buddie = get_buddies(current_block);
        }
        if (log != nullptr){
            log->log("---------The deallocate has finished working--------", logger::severity::debug);
        }
    }
    ~allocator_buddies() override{
        logger* log = *get_logger();
        memory* alloc = *get_allocator();
        if (log != nullptr){
            log->log("---------The destructor has started working-----", logger::severity::debug);
        }
        if (alloc == nullptr){
            ::operator delete (_global_memory);
        }else{
            alloc->deallocate(_global_memory);
        }
        if (log != nullptr){
            log->log("---------The destructor has finished working-----", logger::severity::debug);
        }
    }
};
#endif //LABS_4_SEM_ALLOCATOR_BUDDIES_H
