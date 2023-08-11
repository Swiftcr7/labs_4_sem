#include <iostream>
#include "allocator_buddies.h"
#include <random>
#include <list>

void testing_allocator(
        memory *allocator,
        logger* mlog,
        unsigned int iterations_count)
{
    std::list<void*> allocated_blocks;
    //i want to eat

//    std::random_device rd;
//    auto mtgen = std::mt19937(12345);
//    auto ud = std::uniform_int_distribution<>{0, 2};

    srand(time(nullptr));


    for (size_t i = 0; i < iterations_count; ++i)
    {
        std::string kkk = "--- iter #";
        kkk += std::to_string(i+1);
        kkk += " ---";
        mlog->log(kkk, logger::severity::debug);
        void * ptr;
        size_t yes;
        switch (rand() % 2)
        {
            case 0:
                try
                {
                    yes = rand() % 81 + 20;
                    ptr = reinterpret_cast<void *>(allocator->allocate(yes)); // разность макс и мин с включенными границами + минимальное
                    allocated_blocks.push_back(ptr);
                    std::string msg = "allocation succeeded, size: ";
                    msg += std::to_string(yes);
                    mlog->log(msg, logger::severity::debug);
                }
                catch (std::exception const &ex)
                {
                    std::cout << ex.what() << std::endl;
                }
                break;
            case 1:

                if (allocated_blocks.empty())
                {
                    mlog->log("No blocks to dealloc", logger::severity::debug);
                    break;
                }

                try
                {
                    auto iter = allocated_blocks.begin();
                    std::advance(iter, rand() % allocated_blocks.size());
                    allocator->deallocate(*iter);
                    allocated_blocks.erase(iter);
                    mlog->log("deallocation succeeded", logger::severity::debug);
                }
                catch (std::exception const &ex)
                {
                    std::cout << ex.what() << std::endl;
                }
                break;
        }


    }

    mlog->log("--- END OF ITERATIONS ---", logger::severity::debug);

    while (!allocated_blocks.empty())
    {
        try
        {
            auto iter = allocated_blocks.begin();
            allocator->deallocate(*iter);
            allocated_blocks.erase(iter);
            mlog->log("deallocation succeeded", logger::severity::debug);
        }
        catch (std::exception const &ex)
        {
            std::cout << ex.what() << std::endl;
        }
    }
}

int main() {
    logger_builder* builder = new logger_builder_concrete();
    logger* mylogger = builder->add_stream("console", logger::severity::debug)->add_stream("C:\\Users\\HP\\CLionProjects\\labs_4_sem\\allocator_buddies\\logs.txt", logger::severity::debug)->construct();
    memory* alloc = new allocator_buddies( 60000, mylogger, nullptr);
    testing_allocator(alloc, mylogger,  15000);
    delete alloc;
    delete mylogger;
    delete builder;
    return 0;
}