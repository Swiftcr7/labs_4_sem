#ifndef LABS_4_SEM_ALLOCATOR_ON_SORTED_LIST_H
#define LABS_4_SEM_ALLOCATOR_ON_SORTED_LIST_H
#include "../allocator_operator_new/allocator_operator_new.h"
class allocator_on_sorted_list final : public memory{
private:
    void* _global_memory;
public:
    enum class detour{
        first,
        best,
        worst
    };
private:
    logger** get_logger() const{
        return reinterpret_cast<logger**>(_global_memory);
    }

    memory** get_allocator() const{
        return reinterpret_cast<memory**>(reinterpret_cast<logger**>(_global_memory) + 1);
    }

    detour* get_detour() const{
        return reinterpret_cast<detour*>(reinterpret_cast<memory**>(reinterpret_cast<logger**>(_global_memory) + 1) + 1);
    }

    size_t* get_size() const{
        return reinterpret_cast<size_t*>(reinterpret_cast<detour*>(reinterpret_cast<memory**>(reinterpret_cast<logger**>(_global_memory) + 1) + 1) + 1);
    }

    void** get_pointer_to_first_block() const{
        return reinterpret_cast<void**>(reinterpret_cast<size_t*>(reinterpret_cast<detour*>(reinterpret_cast<memory**>(reinterpret_cast<logger**>(_global_memory) + 1) + 1) + 1) + 1);
    }

    size_t* get_block_size(void* block) const{
        return reinterpret_cast<size_t*>(block);
    }

    void** get_pointer_to_block(void* block) const{
        return reinterpret_cast<void**>(reinterpret_cast<size_t*>(block) + 1);
    }

    void merging() const{
        logger* log = *get_logger();
        if (log != nullptr) log->log("---------The merging has started working---------", logger::severity::debug);
        void* next_block = nullptr;
        auto* current_block = *get_pointer_to_first_block();
        while (current_block != nullptr){
            next_block = *get_pointer_to_block(current_block);
            if ((reinterpret_cast<unsigned char*>(next_block) - reinterpret_cast<unsigned char*>(current_block)) == (*get_block_size(current_block))){
                auto* s = reinterpret_cast<size_t*>(next_block);
                auto** next = reinterpret_cast<void**>(s + 1);
                *get_pointer_to_block(current_block) = *next;
                size_t size = *get_block_size(current_block);
                *get_block_size(current_block) = size + (*get_block_size(next_block));

                *get_pointer_to_block(next_block) = nullptr;
                *s = 0;
                *next = nullptr;
            }else{
                current_block = next_block;
            }
        }
        if (log != nullptr) log->log("---------The merging has finished working---------", logger::severity::debug);
    }
public:
    allocator_on_sorted_list(size_t size, detour det = detour::first, logger* log = nullptr, memory* alloc = nullptr){
        if (log != nullptr) log->log("---------The constructor has started working---------", logger::severity::debug);
        if (alloc == nullptr){
            _global_memory = ::operator new(size + sizeof(void*) + sizeof(detour) + sizeof(logger*) + sizeof(memory*) + sizeof(size_t));
        }else{
            _global_memory = alloc->allocate(size + sizeof(void*) + sizeof(detour) + sizeof(logger*) + sizeof(memory*) + sizeof(size_t));
        }
        *reinterpret_cast<logger**>(_global_memory) = log;
        *reinterpret_cast<memory**>(reinterpret_cast<logger**>(_global_memory) + 1) = alloc;
        *reinterpret_cast<detour*>(reinterpret_cast<memory**>(reinterpret_cast<logger**>(_global_memory) + 1) + 1) = det;
        *reinterpret_cast<size_t*>(reinterpret_cast<detour*>(reinterpret_cast<memory**>(reinterpret_cast<logger**>(_global_memory) + 1) + 1) + 1) = size;
        auto* block = reinterpret_cast<void*>(reinterpret_cast<void**>(reinterpret_cast<size_t*>(reinterpret_cast<detour*>(reinterpret_cast<memory**>(reinterpret_cast<logger**>(_global_memory) + 1) + 1) + 1) + 1) + 1);
        *reinterpret_cast<void**>(reinterpret_cast<size_t*>(reinterpret_cast<detour*>(reinterpret_cast<memory**>(reinterpret_cast<logger**>(_global_memory) + 1) + 1) + 1) + 1) = block;
        *reinterpret_cast<size_t*>(block) = size - sizeof(size_t) - sizeof(void*) - sizeof(memory*) - sizeof(logger*);
        *reinterpret_cast<void**>(reinterpret_cast<size_t*>(block) + 1) = nullptr;
        if (log != nullptr) log->log("---------The constructor has finished working---------", logger::severity::debug);
    }

    void* allocate(size_t target_size) const override {
        logger *log = *get_logger();
        if (log != nullptr) log->log("---------The allocate has started working---------", logger::severity::debug);
        size_t allocation_size = target_size + sizeof(void *) + sizeof(size_t);
        void *allocation_block = nullptr;
        void *previous_block = nullptr;
        void* prev_block = nullptr;
        void *current_block = *get_pointer_to_first_block();
        if (*get_detour() == detour::first) {
            while (current_block != nullptr) {
                if (allocation_size <= (*get_block_size(current_block))) {
                    allocation_block = current_block;
                    prev_block = previous_block;
                    break;
                }
                previous_block = current_block;
                current_block = *get_pointer_to_block(current_block);
            }
        } else if (*get_detour() == detour::best) {
            size_t size_min = *get_size();
            while (current_block != nullptr) {
                if ((allocation_size <= (*get_block_size(current_block))) &&
                    ((*get_block_size(current_block)) < size_min)) {
                    allocation_block = current_block;
                    prev_block = previous_block;
                    size_min = *get_block_size(current_block);
                }
                previous_block = current_block;
                current_block = *get_pointer_to_block(current_block);
            }
        } else if(*get_detour() == detour::worst){
                size_t size_max = 0;
                while (current_block != nullptr){
                    if ((allocation_size <= (*get_block_size(current_block))) && ((*get_block_size(current_block)) > size_max)){
                        allocation_block = current_block;
                        prev_block = previous_block;
                        size_max = *get_block_size(current_block);
                    }
                    previous_block = current_block;
                    current_block = *get_pointer_to_block(current_block);
                }
            }
        if (allocation_block == nullptr){
            throw std::bad_alloc();
        }
        size_t allocation_block_size = *get_block_size(allocation_block);
        if (prev_block == nullptr){
            if (allocation_block_size - allocation_size < sizeof(void*) + sizeof(size_t)){
                *get_pointer_to_first_block() = *get_pointer_to_block(allocation_block);
            }else{
                auto* next_block = reinterpret_cast<void*>(reinterpret_cast<unsigned char*>(allocation_block) + allocation_size);
                *get_block_size(allocation_block) = target_size;
                *get_block_size(next_block) = allocation_block_size - allocation_size;
                *get_pointer_to_block(next_block) = *get_pointer_to_block(allocation_block);
                *get_pointer_to_first_block() = next_block;

            }
            *get_pointer_to_block(allocation_block) = nullptr;
        }else{
            if (allocation_block_size - allocation_size < sizeof(void*) + sizeof(size_t)){
                *get_pointer_to_block(prev_block) = *get_pointer_to_block(allocation_block);
            }else{
                auto* next_block = reinterpret_cast<void*>(reinterpret_cast<unsigned char*>(allocation_block) + allocation_size);
                *get_block_size(allocation_block) = target_size;
                *get_block_size(next_block) = allocation_block_size - allocation_size;
                *get_pointer_to_block(next_block) = *get_pointer_to_block(allocation_block);
                *get_pointer_to_block(prev_block) = next_block;
            }
            *get_pointer_to_block(allocation_block) = nullptr;
        }
        if (log != nullptr) log->log("---------The allocate has finished working---------", logger::severity::debug);
        return reinterpret_cast<void*>(reinterpret_cast<size_t*>(allocation_block) + 1);
    }

    void deallocate(void* result) const override {
        logger* log = *get_logger();
        if (log != nullptr) log->log("---------The deallocate has started working---------", logger::severity::debug);
        if (result == nullptr){
            return;
        }
        auto* block = reinterpret_cast<size_t*>(result) - 1;
        auto* current_block = *get_pointer_to_first_block();
        void* previous_block = nullptr;
        size_t size = *get_block_size(block);
//        std::string result_log = "[ ";
//        for (int i = 0; i < size; i++) {
//            unsigned char a = *(reinterpret_cast<unsigned char *>(result) + i);
//            result_log += std::to_string(static_cast<unsigned short>(a));
//            result_log += " ";
//        }
//        std::string result_msg = "Block to deallocate: " + result_log + "]";
//        if (log != nullptr) {
//            log->log(result_msg, logger::severity::debug);
//        }
        while ((reinterpret_cast<unsigned char*>(block) - reinterpret_cast<unsigned char*>(current_block) > 0) && current_block != nullptr){
            previous_block = current_block;
            current_block = *get_pointer_to_block(current_block);
        }
        if (previous_block == nullptr){
            *get_pointer_to_first_block() = block;
            *get_pointer_to_block(block) = current_block;
        }else{
            *get_pointer_to_block(previous_block) = block;
            *get_pointer_to_block(block) = current_block;
        }
        merging();
        if (log != nullptr) log->log("---------The deallocate has finished working---------", logger::severity::debug);

    }

    ~allocator_on_sorted_list(){
        logger* log = *get_logger();
        memory* alloc = *get_allocator();
        if (log != nullptr) log->log("---------The destructor has started working---------", logger::severity::debug);
        if (alloc != nullptr){
            alloc->deallocate(_global_memory);
        }else{
            ::operator delete(_global_memory);
        }
        if (log != nullptr) log->log("---------The destructor has finished working---------", logger::severity::debug);
    }
};
#endif //LABS_4_SEM_ALLOCATOR_ON_SORTED_LIST_H
