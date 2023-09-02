#include <iostream>
#include <stack>
#include <functional>
#include <vector>
#include <ctime>
#include <limits>
#include "binary_search_tree.h"
#include "../allocator_on_sorted_list//allocator_on_sorted_list.h"


class my_int_comparer
{
public:
    my_int_comparer() = default;

    int operator()(int first, int second){
        return first - second;
    }
};

int main() {
    auto *builder = new logger_builder_concrete();

    logger *logger1 = builder->add_stream("console", logger::severity::debug)
            ->construct();
    memory *alloc = new allocator_on_sorted_list(10000000, allocator_on_sorted_list::detour::first);
    associative_container<int, std::string> *bst = new binary_search_tree<int, std::string, my_int_comparer>(alloc,
                                                                                                             logger1);
//    auto* bst = new binary_search_tree<int, string, my_int_comparer>(allocator, logger1);



//-----------------------------------------

//stress test

    srand(time(nullptr));
    for (int i = 0; i < 15000; ++i) {
        std::cout << "== iteration #" << i + 1 << " ==" << std::endl;
        int action = rand() % 3;
        int bp_mode = rand() % 3;

        int random_key = rand() % 21;
        std::string value_str = "str";
        bool result;
        switch (action) {
            case 0:
                try {
                    logger1->log("inserting " + std::to_string(random_key), logger::severity::debug);
                    bst->insert(random_key, std::move(value_str));
                } catch (const std::logic_error &ex) {
                    logger1->log("Insertion of " + std::to_string(random_key) + " failed, exception message: \"" +
                                 ex.what() + "\"", logger::severity::debug);
                }
                break;
            case 1:
                try {
                    logger1->log("removing " + std::to_string(random_key), logger::severity::debug);
                    bst->remove(random_key);
                } catch (const std::logic_error &ex) {
                    logger1->log(
                            "Removing of " + std::to_string(random_key) + " failed, exception message: \"" + ex.what() +
                            "\"", logger::severity::debug);
                }
                break;
            case 2:
                try {
                    logger1->log("searching " + std::to_string(random_key), logger::severity::debug);
                    associative_container<int, std::string>::key_value_pair p;
                    p._key = random_key;
                    p._value = value_str;
                    result = bst->find(&p);

                    std::cout << "Value by key " << random_key << " == " << result << std::endl;
                } catch (const std::logic_error &ex) {
                    logger1->log("Searching of " + std::to_string(random_key) + " failed, exception message: \"" +
                                 ex.what() + "\"", logger::severity::debug);
                }
        }

        switch (bp_mode) {
            case 0:
                logger1->log("prefix iterator", logger::severity::debug);
                bst->bypass_tree(associative_container<int, std::string>::bypass_detour::prefix);
                break;
            case 1:
                logger1->log("infix iterator", logger::severity::debug);
                bst->bypass_tree(associative_container<int, std::string>::bypass_detour::infix);
                break;
            case 2:
                logger1->log("postfix iterator", logger::severity::debug);
                bst->bypass_tree(associative_container<int, std::string>::bypass_detour::postfix);
                break;
        }
    }
}




