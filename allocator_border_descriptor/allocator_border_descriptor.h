#ifndef LABS_4_SEM_ALLOCATOR_BORDER_DESCRIPTOR_H
#define LABS_4_SEM_ALLOCATOR_BORDER_DESCRIPTOR_H

#include <iostream>
#include <limits.h>
#include "../../untitled4/allocator2.h"   //TODO: fix path!!!!!

class allocator_border_descriptor final : public memory{
private:
    void* _global_memory;
public:
    enum class detour{
        first,
        best,
        worst
    };
private:

    void** get_pointer_to_next_block(void* block) const{
        return reinterpret_cast<void**>(reinterpret_cast<size_t*>(block)+1);
    }

    void** get_pointer_to_past_block(void* block) const {
        return reinterpret_cast<void**>(reinterpret_cast<void**>(reinterpret_cast<size_t*>(block)+1)+1);
    }



    size_t* get_size()const{
        return reinterpret_cast<size_t*>(reinterpret_cast<detour*>(reinterpret_cast<logger**>(reinterpret_cast<memory**>(_global_memory)+1)+1)+1);
    }

    memory** get_allocator()const{
        return reinterpret_cast<memory**>(_global_memory);
    }

    logger** get_logger()const{
        return reinterpret_cast<logger**>(reinterpret_cast<memory**>(_global_memory)+1);
    }

    allocator_border_descriptor::detour* get_detour()const{
        return reinterpret_cast<detour*>(reinterpret_cast<logger**>(reinterpret_cast<memory**>(_global_memory)+1)+1);
    }

    void** get_pointer()const{
        return reinterpret_cast<void**>(reinterpret_cast<size_t*>(reinterpret_cast<detour*>(reinterpret_cast<logger**>(reinterpret_cast<memory**>(_global_memory)+1)+1)+1)+1);
    }


    size_t* get_block_size(void* block) const{
        return reinterpret_cast<size_t*>(block);
    }

    void inner_dealloc(void* res)const {


        auto block = reinterpret_cast<void *>(reinterpret_cast<unsigned char *>(res) - 2 * sizeof(void *) - sizeof(size_t));

        if (*get_pointer_to_past_block(block) == nullptr && *get_pointer_to_next_block(block) == nullptr) {
            *get_pointer() = nullptr;
        } else if (*get_pointer_to_past_block(block) == nullptr && *get_pointer_to_next_block(block) != nullptr) {
            *get_pointer_to_past_block(*get_pointer_to_next_block(block)) = nullptr;
            *get_pointer() = *get_pointer_to_next_block(block);

        } else if (*get_pointer_to_next_block(block) == nullptr && *get_pointer_to_past_block(block) != nullptr) {
            *get_pointer_to_next_block(*get_pointer_to_past_block(block)) = nullptr;
        } else {
            *get_pointer_to_past_block(*get_pointer_to_next_block(block)) = *get_pointer_to_past_block(block);
            *get_pointer_to_next_block(*get_pointer_to_past_block(block)) = *get_pointer_to_next_block(block);
        }

    }


public:
    explicit allocator_border_descriptor(std::size_t size, logger* my_logger = nullptr, memory* alloc = nullptr, allocator_border_descriptor::detour det=detour::first){
        if (alloc == nullptr){
            _global_memory = :: operator new(size+sizeof(logger*)+sizeof(size_t)+sizeof(void*) +sizeof(memory*)+sizeof(detour));
        }else{
            _global_memory=alloc->allocate(size+sizeof(logger*)+sizeof(size_t)+sizeof(void*) +sizeof(memory*)+sizeof(detour));
        }
        *reinterpret_cast<memory**>(_global_memory)=alloc;
        *reinterpret_cast<logger**>(reinterpret_cast<memory**>(_global_memory)+1) = my_logger;
        *reinterpret_cast<detour*>(reinterpret_cast<logger**>(reinterpret_cast<memory**>(_global_memory)+1)+1)=det;
        *reinterpret_cast<size_t*>(reinterpret_cast<detour*>(reinterpret_cast<logger**>(reinterpret_cast<memory**>(_global_memory)+1)+1)+1)=size;
        *reinterpret_cast<void**>(reinterpret_cast<size_t*>(reinterpret_cast<detour*>(reinterpret_cast<logger**>(reinterpret_cast<memory**>(_global_memory)+1)+1)+1)+1)= nullptr;
    }
    void* allocate(size_t target_size)const override{
        if (*get_pointer()== nullptr){
            if (*get_size()>=target_size+sizeof(size_t)+sizeof(void*)+sizeof(void*)){
                auto bloc=reinterpret_cast<void*>(get_pointer()+1);
                *get_block_size(bloc)=target_size;
                *get_pointer_to_past_block(bloc)= nullptr;
                *get_pointer_to_next_block(bloc)= nullptr;
                *get_pointer()=bloc;
                return reinterpret_cast<void*>(get_pointer_to_past_block(bloc)+1);
            }else{
                return nullptr;
            }
        }
        switch (*get_detour()){
            case detour::first:{
                auto current_block= *get_pointer();
                if (reinterpret_cast<unsigned char*>(*get_pointer())-reinterpret_cast<unsigned char*>(reinterpret_cast<void**>(get_pointer())+1)>=target_size+sizeof(size_t)+2*sizeof(void*)){
                    auto bloc=reinterpret_cast<void *>(reinterpret_cast<void**>(get_pointer())+1);
                    *get_block_size(bloc)=target_size;
                    *get_pointer_to_past_block(bloc)= nullptr;
                    *get_pointer_to_past_block(*get_pointer())=bloc;
                    *get_pointer_to_next_block(bloc)=*get_pointer();
                    *get_pointer()=bloc;
                    return reinterpret_cast<void*>(get_pointer_to_past_block(bloc)+1);

                }
                while (*get_pointer_to_next_block(current_block)!= nullptr){
                    if (reinterpret_cast<unsigned char*>(*get_pointer_to_next_block(current_block))-(reinterpret_cast<unsigned char*>(get_pointer_to_past_block(current_block)+1)+(*get_block_size(current_block)))>=target_size+sizeof(size_t)+2*sizeof(void*)){
                        auto pointer=*get_pointer_to_next_block(current_block);
                        *get_block_size(reinterpret_cast<unsigned char*>(get_pointer_to_past_block(current_block)+1)+(*get_block_size(current_block)))=target_size;
                        *get_pointer_to_next_block(reinterpret_cast<unsigned char*>(get_pointer_to_past_block(current_block)+1)+*get_block_size(current_block))=*get_pointer_to_next_block(current_block);
                        *get_pointer_to_past_block(reinterpret_cast<unsigned char*>(get_pointer_to_past_block(current_block)+1)+*get_block_size(current_block))=current_block;
                        *get_pointer_to_next_block(current_block)=reinterpret_cast<unsigned char*>(get_pointer_to_past_block(current_block)+1)+*get_block_size(current_block);
                        *get_pointer_to_past_block(pointer)=reinterpret_cast<unsigned char*>(get_pointer_to_past_block(current_block)+1)+*get_block_size(current_block);
                        return reinterpret_cast<void*>(get_pointer_to_past_block(reinterpret_cast<unsigned char*>(get_pointer_to_past_block(current_block)+1)+*get_block_size(current_block))+1);
                    }else{
                        current_block=*get_pointer_to_next_block(current_block);
                    }
                }
                if (*get_pointer_to_next_block(current_block)== nullptr && (reinterpret_cast<unsigned char*>(reinterpret_cast<void**>(get_pointer())+1)+*get_size()-reinterpret_cast<unsigned char*>(get_pointer_to_past_block(current_block)+1)-*get_block_size(current_block))>=target_size+sizeof(size_t)+2*sizeof(void*)){
                    *get_block_size((reinterpret_cast<unsigned char*>(get_pointer_to_past_block(current_block)+1)+*get_block_size(current_block)))=target_size;
                    *get_pointer_to_next_block((reinterpret_cast<unsigned char*>(get_pointer_to_past_block(current_block)+1)+*get_block_size(current_block)))=nullptr;
                    *get_pointer_to_past_block((reinterpret_cast<unsigned char*>(get_pointer_to_past_block(current_block)+1))+*get_block_size(current_block))=current_block;
                    *get_pointer_to_next_block(current_block)=reinterpret_cast<unsigned char*>(get_pointer_to_past_block(current_block)+1)+*get_block_size(current_block);
                    return reinterpret_cast<void*>(get_pointer_to_past_block(reinterpret_cast<unsigned char*>(get_pointer_to_past_block(current_block)+1)+*get_block_size(current_block))+1);
                }
                return nullptr;

            }
            case detour::best:{
                void* result= nullptr;
                int min_size=INT_MAX;
                auto current_block= *get_pointer();
                if (reinterpret_cast<unsigned char*>(*get_pointer())-reinterpret_cast<unsigned char*>(reinterpret_cast<void**>(get_pointer())+1)>=target_size+sizeof(size_t)+2*sizeof(void*) && reinterpret_cast<unsigned char*>(*get_pointer())-reinterpret_cast<unsigned char*>(reinterpret_cast<void**>(get_pointer())+1)<INT_MAX){
                    auto bloc=reinterpret_cast<void *>(reinterpret_cast<void**>(get_pointer())+1);
                    *get_block_size(bloc)=target_size;
                    *get_pointer_to_past_block(bloc)= nullptr;
                    *get_pointer_to_past_block(*get_pointer())=bloc;
                    *get_pointer_to_next_block(bloc)=*get_pointer();
                    *get_pointer()=bloc;
                    result=reinterpret_cast<void*>(get_pointer_to_past_block(bloc)+1);
                    min_size=reinterpret_cast<unsigned char*>(*get_pointer())-reinterpret_cast<unsigned char*>(reinterpret_cast<void**>(get_pointer())+1);


                }
                while (*get_pointer_to_next_block(current_block)!= nullptr){
                    if (reinterpret_cast<unsigned char*>(*get_pointer_to_next_block(current_block))-(reinterpret_cast<unsigned char*>(get_pointer_to_past_block(current_block)+1)+(*get_block_size(current_block)))>=target_size+sizeof(size_t)+2*sizeof(void*)){
                        if (reinterpret_cast<unsigned char*>(*get_pointer_to_next_block(current_block))-(reinterpret_cast<unsigned char*>(get_pointer_to_past_block(current_block)+1)+(*get_block_size(current_block)))<min_size){
                            if (result!= nullptr){
                                inner_dealloc(result);
                            }
                            auto pointer=*get_pointer_to_next_block(current_block);
                            *get_block_size(reinterpret_cast<unsigned char*>(get_pointer_to_past_block(current_block)+1)+(*get_block_size(current_block)))=target_size;

                            *get_pointer_to_next_block(reinterpret_cast<unsigned char*>(get_pointer_to_past_block(current_block)+1)+*get_block_size(current_block))=*get_pointer_to_next_block(current_block);

                            *get_pointer_to_past_block(reinterpret_cast<unsigned char*>(get_pointer_to_past_block(current_block)+1)+*get_block_size(current_block))=current_block;

                            *get_pointer_to_next_block(current_block)=reinterpret_cast<unsigned char*>(get_pointer_to_past_block(current_block)+1)+*get_block_size(current_block);

                            *get_pointer_to_past_block(pointer)=reinterpret_cast<unsigned char*>(get_pointer_to_past_block(current_block)+1)+*get_block_size(current_block);

                            min_size=reinterpret_cast<unsigned char*>(*get_pointer_to_next_block(current_block))-(reinterpret_cast<unsigned char*>(get_pointer_to_past_block(current_block)+1)+(*get_block_size(current_block)));

                            result=reinterpret_cast<void*>(get_pointer_to_past_block(reinterpret_cast<unsigned char*>(get_pointer_to_past_block(current_block)+1)+*get_block_size(current_block))+1);
                            current_block=*get_pointer_to_next_block(current_block);
                        }else{
                            current_block=*get_pointer_to_next_block(current_block);
                        }
                    }else{
                        current_block=*get_pointer_to_next_block(current_block);
                    }
                }
                if (*get_pointer_to_next_block(current_block)== nullptr && (reinterpret_cast<unsigned char*>(reinterpret_cast<void**>(get_pointer())+1)+*get_size()-reinterpret_cast<unsigned char*>(get_pointer_to_past_block(current_block)+1)-*get_block_size(current_block))>=target_size+sizeof(size_t)+2*sizeof(void*)){
                    if ((reinterpret_cast<unsigned char*>(reinterpret_cast<void**>(get_pointer())+1)+*get_size()-reinterpret_cast<unsigned char*>(get_pointer_to_past_block(current_block)+1)-*get_block_size(current_block))<min_size){
                        if (result!= nullptr){
                            inner_dealloc(result);
                        }
                        *get_block_size((reinterpret_cast<unsigned char*>(get_pointer_to_past_block(current_block)+1)+*get_block_size(current_block)))=target_size;

                        *get_pointer_to_next_block((reinterpret_cast<unsigned char*>(get_pointer_to_past_block(current_block)+1)+*get_block_size(current_block)))=nullptr;

                        *get_pointer_to_past_block((reinterpret_cast<unsigned char*>(get_pointer_to_past_block(current_block)+1))+*get_block_size(current_block))=current_block;

                        *get_pointer_to_next_block(current_block)=reinterpret_cast<unsigned char*>(get_pointer_to_past_block(current_block)+1)+*get_block_size(current_block);

                        return reinterpret_cast<void*>(get_pointer_to_past_block(reinterpret_cast<unsigned char*>(get_pointer_to_past_block(current_block)+1)+*get_block_size(current_block))+1);
                    }

                }
                return result;

            }
            case detour::worst:{
                void* result= nullptr;
                int max_size=0;
                auto current_block= *get_pointer();
                if (reinterpret_cast<unsigned char*>(*get_pointer())-reinterpret_cast<unsigned char*>(reinterpret_cast<void**>(get_pointer())+1)>=target_size+sizeof(size_t)+2*sizeof(void*) && reinterpret_cast<unsigned char*>(*get_pointer())-reinterpret_cast<unsigned char*>(reinterpret_cast<void**>(get_pointer())+1) > max_size){
                    auto bloc=reinterpret_cast<void *>(reinterpret_cast<void**>(get_pointer())+1);
                    *get_block_size(bloc)=target_size;
                    *get_pointer_to_past_block(bloc)= nullptr;
                    *get_pointer_to_past_block(*get_pointer())=bloc;
                    *get_pointer_to_next_block(bloc)=*get_pointer();
                    *get_pointer()=bloc;
                    result=reinterpret_cast<void*>(get_pointer_to_past_block(bloc)+1);
                    max_size= reinterpret_cast<unsigned char*>(*get_pointer()) - reinterpret_cast<unsigned char*>(reinterpret_cast<void**>(get_pointer()) + 1);


                }
                while (*get_pointer_to_next_block(current_block)!= nullptr){
                    if (reinterpret_cast<unsigned char*>(*get_pointer_to_next_block(current_block))-(reinterpret_cast<unsigned char*>(get_pointer_to_past_block(current_block)+1)+(*get_block_size(current_block)))>=target_size+sizeof(size_t)+2*sizeof(void*)){
                        if (reinterpret_cast<unsigned char*>(*get_pointer_to_next_block(current_block))-(reinterpret_cast<unsigned char*>(get_pointer_to_past_block(current_block)+1)+(*get_block_size(current_block))) > max_size){
                            if (result!= nullptr){
                                inner_dealloc(result);
                            }
                            auto pointer=*get_pointer_to_next_block(current_block);
                            *get_block_size(reinterpret_cast<unsigned char*>(get_pointer_to_past_block(current_block)+1)+(*get_block_size(current_block)))=target_size;

                            *get_pointer_to_next_block(reinterpret_cast<unsigned char*>(get_pointer_to_past_block(current_block)+1)+*get_block_size(current_block))=*get_pointer_to_next_block(current_block);

                            *get_pointer_to_past_block(reinterpret_cast<unsigned char*>(get_pointer_to_past_block(current_block)+1)+*get_block_size(current_block))=current_block;

                            *get_pointer_to_next_block(current_block)=reinterpret_cast<unsigned char*>(get_pointer_to_past_block(current_block)+1)+*get_block_size(current_block);

                            *get_pointer_to_past_block(pointer)=reinterpret_cast<unsigned char*>(get_pointer_to_past_block(current_block)+1)+*get_block_size(current_block);

                            max_size= reinterpret_cast<unsigned char*>(*get_pointer_to_next_block(current_block)) - (reinterpret_cast<unsigned char*>(get_pointer_to_past_block(current_block) + 1) + (*get_block_size(current_block)));

                            result=reinterpret_cast<void*>(get_pointer_to_past_block(reinterpret_cast<unsigned char*>(get_pointer_to_past_block(current_block)+1)+*get_block_size(current_block))+1);
                            current_block=*get_pointer_to_next_block(current_block);
                        }else{
                            current_block=*get_pointer_to_next_block(current_block);
                        }
                    }else{
                        current_block=*get_pointer_to_next_block(current_block);
                    }
                }
                if (*get_pointer_to_next_block(current_block)== nullptr && (reinterpret_cast<unsigned char*>(reinterpret_cast<void**>(get_pointer())+1)+*get_size()-reinterpret_cast<unsigned char*>(get_pointer_to_past_block(current_block)+1)-*get_block_size(current_block))>=target_size+sizeof(size_t)+2*sizeof(void*)){
                    if ((reinterpret_cast<unsigned char*>(reinterpret_cast<void**>(get_pointer())+1)+*get_size()-reinterpret_cast<unsigned char*>(get_pointer_to_past_block(current_block)+1)-*get_block_size(current_block)) > max_size){
                        if (result!= nullptr){
                            inner_dealloc(result);
                        }
                        *get_block_size((reinterpret_cast<unsigned char*>(get_pointer_to_past_block(current_block)+1)+*get_block_size(current_block)))=target_size;

                        *get_pointer_to_next_block((reinterpret_cast<unsigned char*>(get_pointer_to_past_block(current_block)+1)+*get_block_size(current_block)))=nullptr;

                        *get_pointer_to_past_block((reinterpret_cast<unsigned char*>(get_pointer_to_past_block(current_block)+1))+*get_block_size(current_block))=current_block;

                        *get_pointer_to_next_block(current_block)=reinterpret_cast<unsigned char*>(get_pointer_to_past_block(current_block)+1)+*get_block_size(current_block);

                        result=reinterpret_cast<void*>(get_pointer_to_past_block(reinterpret_cast<unsigned char*>(get_pointer_to_past_block(current_block)+1)+*get_block_size(current_block))+1);
                    }

                }
                return result;

            }
        }

    }
    void deallocate(void* result) const override{
        if (result!= nullptr) {
            inner_dealloc(result);
            auto block = reinterpret_cast<void *>(reinterpret_cast<unsigned char *>(result) - 2 * sizeof(void *) -sizeof(size_t));

            size_t size = *get_block_size(block);
            std::string result_log = "[ ";
            for (int i = 0; i < size; i++) {
                unsigned char a = *(reinterpret_cast<unsigned char *>(result) + i);
                result_log += std::to_string(static_cast<unsigned short>(a));
                result_log += " ";
            }
            std::string result_msg = "Block to deallocate: " + result_log + "]";
            if (*get_logger() != nullptr) {
                (*get_logger())->log(result_msg, logger::severity::debug);

            }
        }

    }


    ~allocator_border_descriptor(){
        if (*get_allocator()== nullptr){
            ::operator delete(_global_memory);
        }else{
            (*get_allocator())->deallocate(_global_memory);
        }

    }

};

#endif //LABS_4_SEM_ALLOCATOR_BORDER_DESCRIPTOR_H
