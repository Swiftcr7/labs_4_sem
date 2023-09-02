

#ifndef LABS_4_SEM_ALLOCATOR_OPERATOR_NEW_H
#define LABS_4_SEM_ALLOCATOR_OPERATOR_NEW_H
#include <iostream>
#include <string>
#include <cstring>
#include "memory.h"
#include "../../labs_4_sem///logger//logger_builder_concrete.h"
#include <new>
class allocator : public memory
{
private:
    logger* _mylog;

public:
    explicit allocator(logger* mylog = nullptr):
            _mylog(mylog){}


    void* allocate(size_t target_size) const override {
        if (_mylog!= nullptr){
            void* result;
            try{
                result = ::operator new(sizeof(size_t) + target_size);
            }catch(std::bad_alloc& ex) {
                throw ex ;
            }
            *reinterpret_cast<size_t *>(result) = target_size;
            std::ostringstream oss;
            oss<< reinterpret_cast<size_t *>(result) + sizeof(size_t);
            _mylog->log(oss.str(), logger::severity::debug);
            return reinterpret_cast<void *>(reinterpret_cast<size_t *>(result) + 1);
        }else{
            void * result;
            try{
                result = ::operator new (sizeof(size_t) + target_size);
            }catch(std::bad_alloc& ex){
                throw ex;

            }
            *reinterpret_cast<size_t *>(result) = target_size;
            return reinterpret_cast<void *>(reinterpret_cast<size_t *>(result) + 1);
        }
    }

    void deallocate(void* result) const override {
        if(_mylog!= nullptr){
            result = reinterpret_cast<size_t *>(result)-1;
            auto memory_size = *reinterpret_cast<size_t *>(result);
//            std::string result_log = "[ ";
//            for (int i = 0; i < memory_size; i++){
//                unsigned char a = *(reinterpret_cast<unsigned char*>(result) + sizeof(size_t) + i);
//                result_log = result_log + std::to_string(static_cast<unsigned short>(a))+" ";
//            }
//            result_log = "Block to deallocate: " + result_log + "]";
//            std::ostringstream oss;
//            oss<<reinterpret_cast<size_t*>(result) +sizeof (size_t);
//            _mylog->log(oss.str(), logger::severity::debug)->log(result_log, logger::severity::debug);
//            _mylog->log(result_log, logger::severity::debug);

            :: operator delete(result);
        }else{
            result = reinterpret_cast<size_t *>(result)-1;
            :: operator delete(result);
        }
    }
};
#endif //LABS_4_SEM_ALLOCATOR_OPERATOR_NEW_H
