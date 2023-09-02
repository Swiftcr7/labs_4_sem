#ifndef LABS_4_SEM_DATABASE_H
#define LABS_4_SEM_DATABASE_H
#include <iostream>
#include "../allocator_operator_new/allocator_operator_new.h"
#include "../allocator_border_descriptor/allocator_border_descriptor.h"
#include "../allocator_buddies/allocator_buddies.h"
#include "../AVL/AVL.h"
#include <queue>
class database final {
public:
    enum class name_allocators{
        allocator_operator_new,
        allocator_buddies,
        allocator_border_descriptor
    };

    enum class allocator_mode{
        first,
        worst,
        best,
        no
    };

    enum class reporting_format{
        course_paper,
        offset,
        exam
    };

private:
    struct session_key final{
        int id_session;
        int id_student;
        std::string subject;
        reporting_format report_format;

    };

    struct session_value final{
        std::string name_student;
        std::string surname_student;
        std::string patronymic_student;
        std::string exam_date;
        std::string start_date;
        int estimation;
        std::string name_teacher;
        std::string surname_teacher;
        std::string patronymic_teacher;

    };

    class session_key_comparer final{
    public:
        session_key_comparer() = default;

        int operator()(const session_key& key1, const session_key& key2){
            if ((key1.id_session - key2.id_session) != 0){
                return key1.id_session - key2.id_session;
            }else{
                if ((key1.id_student - key2.id_student) != 0){
                    return key1.id_student - key2.id_student;
                }else {
                    if (key1.subject.compare(key2.subject) != 0){
                        return key1.subject.compare(key2.subject);
                    } else{
                        return (int)(key2.report_format != key1.report_format);
                    }

                }
            }
        }
    };

    class string_comparer final{
    public:
        string_comparer() = default;
        int operator()(const std::string& str1, const std::string& str2){
            return str1.compare(str2);
        }
    };

private:
    logger* _logger;
    allocator* _allocator;
    AVL_tree<std::string, memory*, string_comparer>* _allocators_pool;
    AVL_tree<std::string, AVL_tree<std::string, AVL_tree<std::string, associative_container<session_key, session_value>*, string_comparer>*, string_comparer>*, string_comparer>* _database;
public:
    explicit database(logger* log) : _logger(log){
        if (log != nullptr){
            log->log("---------The constructor Database has started working--------", logger::severity::debug);
        }
        _allocator = new allocator(log);
        _allocators_pool = new AVL_tree<std::string, memory*, string_comparer>(_allocator, log);
        _database = new AVL_tree<std::string, AVL_tree<std::string, AVL_tree<std::string, associative_container<session_key, session_value>*, string_comparer>*, string_comparer>*, string_comparer>(_allocator, _logger);
        if (_logger != nullptr){
            _logger->log("---------The constructor Database has finished working--------", logger::severity::debug);
        }
    }
    ~database(){
        if (_logger != nullptr){
            _logger->log("------------The destructor Database has started working-----------", logger::severity::debug);
        }
        delete _allocator;
        reset_database();
        delete _allocators_pool;
        delete _database;
        if (_logger != nullptr){
            _logger->log("------------The destructor Database has finished working-----------", logger::severity::debug);
        }
    }
    database(const database& other) = delete;
    database operator=(const database& other) = delete;
    database(const database&& other) = delete;
    database operator=(const database&& other) = delete;

public:

    void dialogue(){
        if (_logger != nullptr){
            _logger->log("------------------------------------------------------------------------", logger::severity::warning);
            _logger->log("-------------------------The dialogue has begun-------------------------", logger::severity::warning);
        }
        std::string choice = " ";
        while (choice != "4"){
            if (_logger != nullptr){
                _logger->log("------------------------------Select action-----------------------------\n1) Enter a command manually 2) Run commands in a file 3) Reset database 4) Exit database\n(You need to write only a number, without brackets and words)\nInput: ", logger::severity::warning);
            }
            std::cin.clear();
            std::getline(std::cin, choice);
            if (choice == "1"){
                std::string command;
                if (_logger != nullptr) _logger->log("\n1) add pool\n2) delete pool\n3) add scheme\n4) delete scheme\n5) add collection\n6) delete collection\n7) insert\n8) remove\n9) get key information\n10) read range\n11) update key\n(You need to write only a number, without brackets and words)\nCommand:", logger::severity::warning);
                std::cin.clear();
                std::getline(std::cin, command);
                if (command == "1"|| command == "2" || command == "3" || command == "4" || command == "5" || command == "6" || command == "7" || command == "8" || command == "9" || command == "10" || command == "11"){
                    if (command == "1"){
                        if (_logger != nullptr) _logger->log("Enter the name of pool and command exactly as shown below\nAdd pool: (pool name) [{name  of allocator} {size for allocation, if you need it} {allocator mode, if you need it}]\nEnter the name of pool and the command\nCommand: ", logger::severity::warning);
                    }else if (command == "2"){
                        if (_logger != nullptr) _logger->log("Enter the name of pool and command exactly as shown below\nDelete pool: (pool name)\nEnter the name of pool and the command\nCommand: ", logger::severity::warning);
                    }else if (command == "3"){
                        if (_logger != nullptr) _logger->log("Enter the name of pool, name of scheme and command exactly as shown below\nAdd scheme: (pool name) (scheme name)\nEnter the name of pool, name of scheme and command\nCommand: ", logger::severity::warning);
                    }else if (command == "4"){
                       if (_logger != nullptr) _logger->log("Enter the name of pool, name of scheme and command exactly as shown below\nDelete scheme: (pool name) (scheme name)\nEnter the name of pool, name of scheme and command\nCommand: ", logger::severity::warning);
                    }else if (command == "5"){
                        if (_logger != nullptr) _logger->log("Enter the name of pool, name of scheme, name of collection and command exactly as shown below\nAdd collection: (pool name) (scheme name) (collection name)\nEnter the name of pool, name of scheme, name of collection and command\nCommand: ", logger::severity::warning);
                    }else if (command == "6"){
                        if (_logger != nullptr) _logger->log("Enter the name of pool, name of scheme, name of collection and command exactly as shown below\nDelete collection: (pool name) (scheme name) (collection name)\nEnter the name of pool, name of scheme, name of collection and command\nCommand: ", logger::severity::warning);
                    }else if (command == "7"){
                        if (_logger != nullptr) _logger->log("Enter the location map, key and value exactly as shown below\nInsert: (pool name) (scheme name) (collection name)\nKey: (id_session) (id_student) (name subject) (reporting_format)\nValue: (name student) (surname student) (patronymic student) (exam_date) (start date) (estimation) (name teacher) (surname teacher) (patronymic teacher)\nEnter the command and map to the node\nCommand and map: ", logger::severity::warning);
                    }else if (command == "8"){
                        if (_logger != nullptr) _logger->log("Enter the location map, key and value exactly as shown below\nRemove: (pool name) (scheme name) (collection name)\nKey: (id_session) (id_student) (name subject) (reporting_format)\nEnter the command and map to the node\nCommand and map: ", logger::severity::warning);
                    }else if (command == "9"){
                        if (_logger != nullptr) _logger->log("Enter the location map and key exactly as shown below\nGet key information: (pool name) (scheme name) (collection name)\nKey: (id_session) (id_student) (name subject) (reporting_format)\nEnter the command and map to the node\nCommand and map: ", logger::severity::warning);
                    }else if (command == "10"){
                        if (_logger != nullptr) _logger->log("Enter the location map and keys exactly as shown below\nRead range: (pool name) (scheme name) (collection name)\n[Key{beginning}: (id_session) (id_student) (name subject) (reporting_format)]\n[Key{ending}: (id_session) (id_student) (name subject) (reporting_format)]\nIf you want the output from the very beginning or to the very end, leave [] empty in the appropriate places. Words {beginning} and {ending} do not need to be entered", logger::severity::warning);
                    }else if (command == "11"){
                        if (_logger != nullptr) _logger->log("Enter the location map, key and value exactly as shown below\nUpdate key: (pool name) (scheme name) (collection name)\nKey: (id_session) (id_student) (name subject) (reporting_format)\nValue: (name student) (surname student) (patronymic student) (exam_date) (start date) (estimation) (name teacher) (surname teacher) (patronymic teacher)\nEnter the command and map to the node\nCommand and map: ", logger::severity::warning);
                    }
                    std::string command_str;
                    std::string key_str;
                    std::string value_str;
                    std::cin.clear();
                    std::getline(std::cin, command_str);
                    if (command == "7" || command == "11" || command == "10"){
                        if (_logger != nullptr) _logger->log("Enter the key\nkey: ", logger::severity::warning);
                        std::cin.clear();
                        std::getline(std::cin, key_str);
                        if (command == "10"){
                            if (_logger != nullptr) _logger->log("Enter the key\nKey: ", logger::severity::warning);
                        }else{
                            if (_logger != nullptr) _logger->log("Enter the value\nvalue: ", logger::severity::warning);
                        }
                        std::cin.clear();
                        std::getline(std::cin, value_str);
                    }else if (command == "8" || command == "9"){
                        if (_logger != nullptr) _logger->log("Enter the key\nkey: ", logger::severity::warning);
                        std::cin.clear();
                        std::getline(std::cin, key_str);
                        value_str = "";
                    }else{
                        key_str = "";
                        value_str = "";
                    }
                    try{
                        parsing_enter(command_str, key_str, value_str);
                        if (_logger != nullptr) _logger->log("The command has been successfully executed. You can enter the data again", logger::severity::warning);
                        continue;
                    }catch (const std::logic_error& er){
                        std::string message = "An error occurred during the execution of the program\n";
                        message += er.what();
                        if (_logger != nullptr) _logger->log(message, logger::severity::warning);
                        if (_logger != nullptr) _logger->log("You can enter the data again", logger::severity::warning);
                        continue;
                    }
                }else{
                    if (_logger != nullptr) _logger->log("Incorrect input, check the data and try again", logger::severity::warning);
                    continue;
                }
            }else if (choice == "2"){
                if (_logger != nullptr) _logger->log("Enter the path to the file", logger::severity::warning);
                std::cin.clear();
                std::string way;
                std::getline(std::cin, way);
                if (way.empty()){
                    if (_logger != nullptr) _logger->log("The path is empty", logger::severity::warning);
                    continue;
                }
                try{
                    parsing_in_file(way);
                }catch (const std::logic_error& er){
                    std::string flag;
                    if (_logger != nullptr) _logger->log(er.what(), logger::severity::warning);
                    if (_logger != nullptr) _logger->log("Error in parsing file!\nSelect action\n1) Continue 2) Reset and continues 3) Exit database\n(You need to write only a number, without brackets and words)\nInput:", logger::severity::warning);
                    std::cin.clear();
                    std::getline(std::cin, flag);
                    if (flag == "1"){
                        continue;
                    }else if (flag == "2"){
                        if (_logger != nullptr) _logger->log("Reset database", logger::severity::warning);
                        reset_database();
                        continue;
                    }else if (flag == "3"){
                        if (_logger != nullptr) _logger->log("------------------------Exiting----------------------------", logger::severity::warning);
                        break;
                    }else{
                        if (_logger != nullptr) _logger->log("Error: incorrect flag", logger::severity::warning);
                        continue;
                    }
                }
                std::string flag;
                if (_logger != nullptr) _logger->log("Parsing file finished successfully!\nSelect action\n1) Continue 2)  Exit database\n(You need to write only a number, without brackets and words)\nInput:", logger::severity::warning);
                std::cin.clear();
                std::getline(std::cin, flag);
                if (flag == "1"){
                    continue;
                }else if (flag == "2"){
                    if (_logger != nullptr) _logger->log("------------------------Exiting----------------------------", logger::severity::warning);
                    break;
                }else{
                    if (_logger != nullptr) _logger->log("Error: incorrect flag", logger::severity::warning);
                    continue;
                }
            }else if (choice == "3"){
                reset_database();
                if (_logger != nullptr) _logger->log("The command has been successfully executed. You can enter the data again", logger::severity::warning);
                continue;
            }else if (choice == "4"){
                if (_logger != nullptr) _logger->log("-------------------Exiting--------------------", logger::severity::warning);
                break;
            }else{
                if (_logger != nullptr) _logger->log("--------------------Incorrect input.Exiting-------------------", logger::severity::warning);
                break;
            }
        }
        if (_logger != nullptr) _logger->log("--------------------The dialogue has over --------------------", logger::severity::warning);
        if (_logger != nullptr) _logger->log("--------------------------------------------------------------", logger::severity::warning);
    }

private:

    AVL_tree<std::string, AVL_tree<std::string, associative_container<session_key, session_value>*, string_comparer>*, string_comparer>* get_pool(const std::string& pool_name){
        AVL_tree<std::string, AVL_tree<std::string, associative_container<session_key, session_value>*, string_comparer>*, string_comparer>* pool;
        try{
            pool = _database->get(pool_name);
        }catch (const std::logic_error& error){
            std::string message = "Error: Pool - ";
            message += pool_name;
            message += " not found in the database";
            throw std::logic_error(message);

        }
        return pool;
    }

    static AVL_tree<std::string, associative_container<session_key, session_value>*, string_comparer>* get_scheme(const std::string& scheme_name, AVL_tree<std::string, AVL_tree<std::string, associative_container<session_key, session_value>*, string_comparer>*, string_comparer>* pool){
        AVL_tree<std::string, associative_container<session_key, session_value>*, string_comparer>* scheme;
        try{
            scheme = pool->get(scheme_name);
        }catch (const std::logic_error& error){
            std::string message = "Error: Scheme - ";
            message += scheme_name;
            message += " not found in the pool";
            throw std::logic_error(message);
        }
        return scheme;
    }

    static associative_container<session_key, session_value>* get_collection(const std::string& collection_name, AVL_tree<std::string, associative_container<session_key, session_value>*, string_comparer>* scheme){
        associative_container<session_key, session_value>* collection;
        try{
            collection = scheme->get(collection_name);
        }catch (const std::logic_error& error){
            std::string message = "Error: collection - ";
            message += collection_name;
            message += " not found in the scheme";
            throw std::logic_error(message);
        }
        return collection;
    }

//----------------------------------------COMMANDS BEGINNING----------------------------------------------------

    void add_pool(const std::string& pool_name, size_t size, name_allocators name_allocator, allocator_mode mode = allocator_mode::no ){
        //check maybe bug
        if (_logger != nullptr) _logger->log("-----------------Add pool has started working-----------------", logger::severity::debug);
        int flag = 1;
        try{
            _database->get(pool_name);
        }catch(const std::logic_error& ex) {
            flag = 0;
        }

        if (flag){
            std::string message = "Error: Pool - ";
            message += pool_name;
            message += " already available";
            throw std::logic_error(message);

        }
        memory* alloc;
        if (name_allocator == name_allocators::allocator_buddies){
            alloc = new allocator_buddies(size, _logger);
        }else if (name_allocator == name_allocators::allocator_operator_new){
            alloc = new allocator(_logger);
        }else if (name_allocator == name_allocators::allocator_border_descriptor){
            if (mode == allocator_mode::first){
                alloc = new allocator_border_descriptor(size, _logger);
            }else if (mode == allocator_mode::best){
                alloc = new allocator_border_descriptor(size, _logger, nullptr, allocator_border_descriptor::detour::best);
            }else if (mode == allocator_mode::worst){
                alloc = new allocator_border_descriptor(size, _logger, nullptr, allocator_border_descriptor::detour::worst);
            }else{
                throw std::logic_error("Incorrect mode");
            }
        }
        auto* pool_tree = reinterpret_cast<AVL_tree<std::string, AVL_tree<std::string, associative_container<session_key, session_value>*, string_comparer>*, string_comparer>*>(alloc->allocate(sizeof(AVL_tree<std::string, AVL_tree<std::string, associative_container<session_key, session_value>*, string_comparer>*, string_comparer>)));
        if (pool_tree == nullptr){
            throw std::logic_error("Not enough memory");
        }
        new (pool_tree) AVL_tree<std::string, AVL_tree<std::string, associative_container<session_key, session_value>*, string_comparer>*, string_comparer> (alloc, _logger);
        _database->insert(pool_name, pool_tree);
        _allocators_pool->insert(pool_name, alloc);
        if (_logger != nullptr) _logger->log("----------------Add pool has finished working-----------------", logger::severity::debug);

    }

    void add_scheme(const std::string& pool_name, const std::string& scheme_name){
        if (_logger != nullptr) _logger->log("------------Add scheme has started working------------", logger::severity::debug);
        AVL_tree<std::string, AVL_tree<std::string, associative_container<session_key, session_value>*, string_comparer>*, string_comparer>* pool;
        try{
            pool = get_pool(pool_name);
        }catch(const std::logic_error& error){
            throw error;
        }
        bool flag = true;
        try{
            pool->get(scheme_name);
        }catch(const std::logic_error& error){
            flag = false;
        }
        if (flag){
            std::string message = "Error:In Pool - ";
            message += pool_name;
            message += ", scheme - ";
            message += scheme_name;
            message += " already available";
            throw std::logic_error(message);
        }
        memory* alloc = _allocators_pool->get(pool_name);
        auto* scheme_tree = reinterpret_cast<AVL_tree<std::string, associative_container<session_key, session_value>*, string_comparer>*>(alloc->allocate(sizeof(AVL_tree<std::string, associative_container<session_key, session_value>*, string_comparer>)));
        if (scheme_tree == nullptr){
            throw std::logic_error("Not enough memory");
        }
        new (scheme_tree) AVL_tree<std::string, associative_container<session_key, session_value>*, string_comparer>(alloc, _logger);
        pool->insert(scheme_name, scheme_tree);
        if (_logger != nullptr) _logger->log("------------Add scheme has finished working------------", logger::severity::debug);
    }

    void add_collection(const std::string& pool_name, const std::string& scheme_name, const std::string& collection_name){
        if (_logger != nullptr) _logger->log("------------Add collection has started working------------", logger::severity::debug);
        AVL_tree<std::string, AVL_tree<std::string, associative_container<session_key, session_value>*, string_comparer>*, string_comparer>* pool;
        AVL_tree<std::string, associative_container<session_key, session_value>*, string_comparer>* scheme;
        try{
            pool = get_pool(pool_name);
            scheme = get_scheme(scheme_name, pool);
        }catch(const std::logic_error& error){
            throw error;
        }
        bool flag = true;
        try{
            scheme->get(collection_name);
        }catch(const std::logic_error& error){
            flag = false;
        }
        if (flag){
            std::string message = "Error:In Pool - ";
            message += pool_name;
            message += ",in scheme - ";
            message += scheme_name;
            message += ", collection - ";
            message += collection_name;
            message += " already available";
            throw std::logic_error(message);
        }
        memory* alloc = _allocators_pool->get(pool_name);
        auto* tree_collection = reinterpret_cast<AVL_tree<session_key, session_value, session_key_comparer>*>(alloc->allocate(sizeof(AVL_tree<session_key, session_value, session_key_comparer>)));
        if (tree_collection == nullptr){
            throw std::logic_error("Not enough memory");
        }
        new (tree_collection) AVL_tree<session_key, session_value, session_key_comparer>(alloc, _logger);
        scheme->insert(collection_name, tree_collection);
        if (_logger != nullptr) _logger->log("------------Add collection has finished working------------", logger::severity::debug);
    }

    void delete_collection(const std::string& pool_name, const std::string& scheme_name, const std::string& collection_name){
        if (_logger != nullptr) _logger->log("------------Delete collection has started working------------", logger::severity::debug);
        AVL_tree<std::string, AVL_tree<std::string, associative_container<session_key, session_value>*, string_comparer>*, string_comparer>* pool;
        AVL_tree<std::string, associative_container<session_key, session_value>*, string_comparer>* scheme;
        associative_container<session_key, session_value>* collection;
        try{
            pool = get_pool(pool_name);
            scheme = get_scheme(scheme_name, pool);
            collection = get_collection(collection_name, scheme);
        }catch (const std::logic_error& er){
            throw er;
        }
        collection->~associative_container();
        memory* alloc = _allocators_pool->get(pool_name);
        alloc->deallocate(reinterpret_cast<void*>(collection));
        scheme->remove(collection_name);
        if (_logger != nullptr) _logger->log("------------Delete collection has finished working------------", logger::severity::debug);
    }

    void delete_scheme(const std::string& pool_name, const std::string& scheme_name){
        if (_logger != nullptr) _logger->log("------------Delete scheme has started working------------", logger::severity::debug);
        AVL_tree<std::string, AVL_tree<std::string, associative_container<session_key, session_value>*, string_comparer>*, string_comparer>* pool;
        AVL_tree<std::string, associative_container<session_key, session_value>*, string_comparer>* scheme;
        try{
            pool = get_pool(pool_name);
            scheme = get_scheme(scheme_name, pool);
        }catch (const std::logic_error& er){
            throw er;
        }
        //TODO in the bst check postfix
        for (auto i = scheme->postfix_it_begin(); i != scheme->postfix_it_end(); i++){
            delete_collection( pool_name, scheme_name,std::get<1>(*i));
        }
        scheme->~AVL_tree();
        memory* alloc = _allocators_pool->get(pool_name);
        alloc->deallocate(reinterpret_cast<void*>(scheme));
        pool->remove(scheme_name);
        if (_logger != nullptr) _logger->log("------------Delete scheme has finished working------------", logger::severity::debug);
    }

    void delete_pool(const std::string& pool_name){
        if (_logger != nullptr) _logger->log("------------Delete pool has started working------------", logger::severity::debug);
        AVL_tree<std::string, AVL_tree<std::string, associative_container<session_key, session_value>*, string_comparer>*, string_comparer>* pool;
        try{
            pool = get_pool(pool_name);
        }catch (const std::logic_error& er){
            throw er;
        }
        for (auto i = pool->postfix_it_begin(); i != pool->postfix_it_end(); i++){
            delete_scheme( pool_name, std::get<1>(*i));
        }
        pool->~AVL_tree();
        memory* alloc = _allocators_pool->get(pool_name);
        alloc->deallocate(reinterpret_cast<void*>(pool));
        _allocators_pool->remove(pool_name);
        _database->remove(pool_name);
        if (_logger != nullptr) _logger->log("------------Delete pool has finished working------------", logger::severity::debug);
    }

    void insert(const session_key& key, const session_value& value, const std::string& pool_name, const std::string& scheme_name, const std::string& collection_name){
        if (_logger != nullptr) _logger->log("------------Insert has started working------------", logger::severity::debug);
        AVL_tree<std::string, AVL_tree<std::string, associative_container<session_key, session_value>*, string_comparer>*, string_comparer>* pool;
        AVL_tree<std::string, associative_container<session_key, session_value>*, string_comparer>* scheme;
        associative_container<session_key, session_value>* collection;
        try{
            pool = get_pool(pool_name);
            scheme = get_scheme(scheme_name, pool);
            collection = get_collection(collection_name, scheme);
        }catch (const std::logic_error& er){
            throw er;
        }
        try{
            collection->insert(key, value);
        }catch(const std::logic_error& er) {
            std::string message = "Insert error: ";
            message += er.what();
            throw std::logic_error(message);
        }
        if (_logger != nullptr) _logger->log("------------Insert has finished working------------", logger::severity::debug);
    }

    session_value get_key_information(const session_key& key, const std::string& pool_name, const std::string& scheme_name, const std::string& collection_name){
        if (_logger != nullptr) _logger->log("------------Get key information has started working------------", logger::severity::debug);
        AVL_tree<std::string, AVL_tree<std::string, associative_container<session_key, session_value>*, string_comparer>*, string_comparer>* pool;
        AVL_tree<std::string, associative_container<session_key, session_value>*, string_comparer>* scheme;
        associative_container<session_key, session_value>* collection;
        try{
            pool = get_pool(pool_name);
            scheme = get_scheme(scheme_name, pool);
            collection = get_collection(collection_name, scheme);
        }catch (const std::logic_error& er){
            throw er;
        }

        try{
            session_value value = collection->get(key);
            if (_logger != nullptr) _logger->log("------------Get key information has finished working------------", logger::severity::debug);
            return value;
        }catch (const std::logic_error& er){
            std::string message = "Get key information error: in collection";
            message += collection_name;
            throw std::logic_error(message);
        }

    }

    void remove(const session_key& key, const std::string& pool_name, const std::string& scheme_name, const std::string& collection_name){
        if (_logger != nullptr) _logger->log("------------Get key information has started working------------", logger::severity::debug);
        AVL_tree<std::string, AVL_tree<std::string, associative_container<session_key, session_value>*, string_comparer>*, string_comparer>* pool;
        AVL_tree<std::string, associative_container<session_key, session_value>*, string_comparer>* scheme;
        associative_container<session_key, session_value>* collection;
        try{
            pool = get_pool(pool_name);
            scheme = get_scheme(scheme_name, pool);
            collection = get_collection(collection_name, scheme);
        }catch (const std::logic_error& er){
            throw er;
        }
        try{
            session_value remove_value = collection->remove(key);
//            flyweight_for_string::get_instance().flyweight_to_remove(remove_value.name_student);
//            flyweight_for_string::get_instance().flyweight_to_remove(remove_value.surname_student);
//            flyweight_for_string::get_instance().flyweight_to_remove(remove_value.patronymic_student);
//            flyweight_for_string::get_instance().flyweight_to_remove(remove_value.exam_date);
//            flyweight_for_string::get_instance().flyweight_to_remove(remove_value.start_date);
//            flyweight_for_string::get_instance().flyweight_to_remove(remove_value.name_teacher);
//            flyweight_for_string::get_instance().flyweight_to_remove(remove_value.surname_teacher);
//            flyweight_for_string::get_instance().flyweight_to_remove(remove_value.patronymic_teacher);
        }catch (const std::logic_error& er){
            std::string message = "Remove error:";
            message += er.what();
            throw std::logic_error(message);
        }
        if (_logger != nullptr) _logger->log("------------Remove has finished working------------", logger::severity::debug);
    }

    void update_key(const session_key& key, const session_value& value, const std::string& pool_name, const std::string& scheme_name, const std::string& collection_name) {
        if (_logger != nullptr)
            _logger->log("------------Update key has started working------------", logger::severity::debug);
        try {
            this->remove(key, pool_name, scheme_name, collection_name);
            this->insert(key, value, pool_name, scheme_name, collection_name);
        } catch (const std::logic_error &er) {

            throw er;
        }
        if (_logger != nullptr)
            _logger->log("------------Update key has finished working------------", logger::severity::debug);
    }

    void reset_database(){
        if (_logger != nullptr) _logger->log("------------Reset database has started working------------", logger::severity::debug);
        for (auto i = _database->postfix_it_begin(); i != _database->postfix_it_end(); i++){
            delete_pool(std::get<1>(*i));
        }
        if (_logger != nullptr) _logger->log("------------Reset database has finished working------------", logger::severity::debug);
    }

    std::queue<std::tuple<session_key, session_value>> read_range(const session_key* key1, const session_key* key2, const std::string& pool_name, const std::string& scheme_name, const std::string& collection_name){
        if (_logger != nullptr) _logger->log("------------Read range has started working------------", logger::severity::debug);
        AVL_tree<std::string, AVL_tree<std::string, associative_container<session_key, session_value>*, string_comparer>*, string_comparer>* pool;
        AVL_tree<std::string, associative_container<session_key, session_value>*, string_comparer>* scheme;
        associative_container<session_key, session_value>* collection;
        try{
            pool = get_pool(pool_name);
            scheme = get_scheme(scheme_name, pool);
            collection = get_collection(collection_name, scheme);
        }catch (const std::logic_error& er){
            throw er;
        }
        bool flag = true;
        std::queue<std::tuple<session_key, session_value>> Queue = std::queue<std::tuple<session_key, session_value>>();
        auto* collection_bst = dynamic_cast<binary_search_tree<session_key, session_value, session_key_comparer>*>(collection);
        auto it = collection_bst->infix_it_begin();
        auto comparator = session_key_comparer();
        if (key1 != nullptr){
            while (comparator(std::get<1>(*it), *key1) != 0 && it != dynamic_cast<binary_search_tree<session_key, session_value, session_key_comparer>*>(collection)->infix_it_end()){
                it++;
            }
            if (it == collection_bst->infix_it_end()){
                flag = false;
            }
        }
        if (flag){
            if (key2 != nullptr){
                while (comparator(std::get<1>(*it), *key2) != 0 && it != collection_bst->infix_it_end()){
                    Queue.emplace(std::get<1>(*it), std::get<2>(*it));
                    it++;
                }
                if (it != collection_bst->infix_it_end()){
                    Queue.emplace(std::get<1>(*it), std::get<2>(*it));
                }
            }else{
                while (it != collection_bst->infix_it_end()){
                    Queue.emplace(std::get<1>(*it), std::get<2>(*it));
                    it++;
                }
            }
        }
        return Queue;
        if (_logger != nullptr) _logger->log("------------Read range has finished working------------", logger::severity::debug);
    }

    //------------------------------COMMAND END-------------------------------------------------------------------

    static std::string session_key_to_string(const session_key& key){
        std::string result = "Key: (";
        result += std::to_string(key.id_session);
        result += ") (";
        result += std::to_string(key.id_student);
        result += ") (";
        result += key.subject;
        result += ") (";
        if (key.report_format == reporting_format::course_paper){
            result += "course_paper";
        }else if (key.report_format == reporting_format::exam){
            result += "exam";
        }else if (key.report_format == reporting_format::offset){
            result += "offset";
        }
        result += ") ";
        return result;
    }

    static std::string session_value_to_string(const session_value& value){
        std::string result = "Value: (";
        result += value.name_student;
        result += ") (";
        result += value.surname_student;
        result += ") (";
        result += value.patronymic_student;
        result += ") (";
        result += value.exam_date;
        result += ") (";
        result += value.start_date;
        result += ") (";
        result += std::to_string(value.estimation);
        result += ") (";
        result += value.name_teacher;
        result += ") (";
        result += value.surname_teacher;
        result += ") (";
        result += value.patronymic_teacher;
        result += ") ";
        return result;

    }

    static session_key parsing_session_key(const std::string& key_str){
        std::string key;
        session_key result;
        std::string subject;
        std::string id_session_str;
        std::string id_student_str;
        std::string report_format;
        reporting_format format;
        size_t position1;
        size_t position2;
        int id_session;
        int id_student;
        if (key_str.find("Key: ") != 0){
            throw std::logic_error("Incorrect input: not enough 'Key: ' or it is in the wrong place");
        }
        key = key_str.substr(5);
        try{
            id_session_str = parsing_string_format(key, "id_session");
            id_student_str = parsing_string_format(key, "id_student");
            subject = parsing_string_format(key, "subject");
        }catch (const std::logic_error& er){
            throw er;
        }
        for (auto i : id_session_str){
            if (!isdigit(i)){
                throw std::logic_error("Incorrect format id_session");
            }
        }
        id_session = std::stoi(id_session_str);
        for (auto i : id_student_str){
            if (!isdigit(i)){
                throw std::logic_error("Incorrect format id_student");
            }
        }
        id_student = std::stoi(id_student_str);
        position1 = key.find('(');
        if (position1 != 0){
            throw std::logic_error("Incorrect input: not enough ( or it is in the wrong place");
        }
        position2 = key.find(')');
        if (position2 == std::string::npos){
            throw std::logic_error("Incorrect input: not enough ) or it is in the wrong place");
        }
        report_format = key.substr(position1+1, position2 - 1);
        if (key.empty()){
            throw std::logic_error("Reporting format not entered");
        }
        if (report_format == "course_paper"){
            format = reporting_format::course_paper;
        }else if (report_format == "offset"){
            format = reporting_format::offset;
        }else if (report_format == "exam"){
            format = reporting_format::exam;
        }else{
            throw std::logic_error("Incorrect reporting format");
        }
        result = {id_session, id_student, subject, format};
        return result;
    }

    static session_value paring_session_value(const std::string& value_str, const session_key& key){
        size_t position1;
        size_t position2;
        std::string value;
        std::string name_student;
        std::string surname_student;
        std::string patronymic_student;
        std::string exam_date;
        std::string start_date;
        int estimation;
        std::string estimation_str;
        std::string name_teacher;
        std::string surname_teacher;
        std::string patronymic_teacher;
        if (value_str.find("Value: ") != 0){
            throw std::logic_error("Incorrect input: not enough 'Value: ' or it is in the wrong place");
        }
        value = value_str.substr(7);
        try{
            name_student = parsing_string_format(value, "name student");
            surname_student = parsing_string_format(value, "surname student");
            patronymic_student = parsing_string_format(value, "patronymic student");
            exam_date = parsing_string_format(value, "exam date");
            start_date = parsing_string_format(value, "start date");
            estimation_str = parsing_string_format(value, "estimation str");
            name_teacher = parsing_string_format(value, "name teacher");
            surname_teacher = parsing_string_format(value, "surname teacher");
        }catch (const std::logic_error& er){
            throw er;
        }
        position1 = value.find('(');
        if (position1 != 0){
            throw std::logic_error("Incorrect input: not enough ( or it is in the wrong place");
        }
        position2 = value.find(')');
        if (position2 == std::string::npos){
            throw std::logic_error("Incorrect input: not enough ) or it is in the wrong place");
        }
        patronymic_teacher = value.substr(position1+1, position2 - 1);
        if (value.empty()){
            throw std::logic_error("Patronymic teacher not entered");
        }
        for (auto i : estimation_str){
            if (!isdigit(i)){
                throw std::logic_error("Incorrect input estimation");
            }
        }
        estimation = std::stoi(estimation_str);
        if (key.report_format == reporting_format::exam || key.report_format == reporting_format::course_paper){
            if (estimation < 2 || estimation > 5){
                throw std::logic_error("Incorrect input estimation");
            }
        }else{
            if (estimation < 0 || estimation > 1){
                throw std::logic_error("Incorrect input estimation");
            }
        }
        session_value v = {name_student, surname_student, patronymic_student, exam_date, start_date, estimation, name_teacher, surname_teacher, patronymic_teacher};
        return v;

    }

    static std::string parsing_string_format(std::string& current_str, const std::string& find_str){
        std::string result;
        size_t position1;
        size_t position2;
        position1 = current_str.find('(');
        if (position1 != 0){
            throw std::logic_error("Incorrect input: not enough ( or it is in the wrong place");
        }
        position2 = current_str.find(") ");
        if (position2 == std::string::npos){
            throw std::logic_error("Incorrect input: not enough ) or it is in the wrong place");
        }
        result = current_str.substr(position1 + 1, position2-1);
        if (result.empty()){
            std::string massage = "Not entered: ";
            massage += find_str;
            throw std::logic_error(massage);
        }
        if (position2+2 > current_str.length()){
            throw std::logic_error("Incorrect input");
        }
        current_str = current_str.substr(position2 + 2);
        return result;
    }

    static void parsing_full_map(std::string& pool_name, std::string& scheme_name, std::string& collection_name,std::string& command){
        size_t position1;
        size_t position2;
        try{
            pool_name = parsing_string_format(command, "pool name");
            scheme_name = parsing_string_format(command, "scheme name");
        }catch (const std::logic_error& er){
            throw er;
        }
        position1 = command.find('(');
        if (position1 != 0){
            throw std::logic_error("Incorrect input: not enough ( or it is in the wrong place");
        }
        position2 = command.find(')');
        if (position2 == std::string::npos){
            throw std::logic_error("Incorrect input: not enough ) or it is in the wrong place");
        }
        collection_name = command.substr(position1+1, position2 - 1);
        if (pool_name.empty()){
            throw std::logic_error("Scheme name not entered");
        }

    }


    void parsing_enter(const std::string& command_str, std::string& key_str, std::string& value_str){
        size_t position1;
        size_t position2;
        std::string command;
        if (command_str.find("Add pool: ") == 0){
            command = command_str.substr(10);
            name_allocators name_allocator;
            allocator_mode mode;
            size_t size_allocator;
            std::string pool_name;
            try{
                pool_name = parsing_string_format(command, "Pool name");
            }catch (const std::logic_error& er){
                throw er;
            }
            position1 = command.find("[ ");
            if (position1 != 0){
                throw std::logic_error("Incorrect input: not enough [ or it is in the wrong place");
            }
            command = command.substr(2);
            position1 = command.find('{');
            if (position1 != 0){
                throw std::logic_error("Incorrect input: not enough { or it is in the wrong place");
            }
            position2 = command.find("} ");
            if (position2 == std::string::npos){
                throw std::logic_error("Incorrect input: not enough } or it is in the wrong place");
            }
            std::string name_allocator_str = command.substr(position1+1, position2-1);
            if (name_allocator_str.empty()){
                throw std::logic_error("Incorrectly entered the name of the allocator");
            }
            if (name_allocator_str == "allocator_operator_new"){
                name_allocator = name_allocators::allocator_operator_new;
            }else if (name_allocator_str == "allocator_buddies"){
                name_allocator = name_allocators::allocator_buddies;
            }else if (name_allocator_str == "allocator_border_descriptor"){
                name_allocator = name_allocators::allocator_border_descriptor;
            }else{
                throw std::logic_error("Incorrect input of the allocator name");
            }
            command = command.substr(position2+2);
            if (name_allocator != name_allocators::allocator_operator_new){
                position1 = command.find('{');
                if (position1 != 0){
                    throw std::logic_error("Incorrect input: not enough { or it is in the wrong place");
                }
                position2 = command.find("} ");
                if (position2 == std::string::npos){
                    throw std::logic_error("Incorrect input: not enough } or it is in the wrong place");
                }
                std::string size = command.substr(position1 + 1, position2 - 1);
                if (size.empty()){
                    throw std::logic_error("Size not entered");
                }
                for (char i : size){
                    if (!isdigit(i)){
                        throw std::logic_error("Incorrect size entered");
                    }
                }
                size_allocator = std::stoi(size);
                command = command.substr(position2 + 2);
            }else{
                size_allocator = 0;
                position1 = command.find(']');
                if (position1 != 0){
                    throw std::logic_error("Incorrect input: not enough ] or it is in the wrong place");
                }
            }

            if (name_allocator == name_allocators::allocator_border_descriptor){
                position1 = command.find('{');
                if (position1 != 0){
                    throw std::logic_error("Incorrect input: not enough { or it is in the wrong place");
                }
                position2 = command.find("} ");
                if (position2 == std::string::npos){
                    throw std::logic_error("Incorrect input: not enough } or it is in the wrong place");
                }
                std::string mode_str = command.substr(position1 + 1, position2 - 1);
                if (mode_str.empty()){
                    throw std::logic_error("Mode not entered");
                }
                if (mode_str == "first"){
                    mode = allocator_mode::first;
                }else if (mode_str == "best"){
                    mode = allocator_mode::best;
                }else if (mode_str == "worst"){
                    mode = allocator_mode::worst;
                }else{
                    throw std::logic_error("Incorrect mode entered");
                }
                command = command.substr(position2 + 2);
                position1 = command.find(']');
                if (position1 != 0){
                    throw std::logic_error("Incorrect input: not enough ] or it is in the wrong place");
                }

            }else{
                mode = allocator_mode::no;
                position1 = command.find(']');
                if (position1 != 0){
                    throw std::logic_error("Incorrect input: not enough ] or it is in the wrong place");
                }
            }
            try{
                add_pool(pool_name, size_allocator, name_allocator, mode);
            }catch (const std::logic_error& er){
                throw er;
            }
            std::string message = "You have successfully added pool: ";
            message += pool_name;
            if (_logger != nullptr) _logger->log(message, logger::severity::warning);

        }else if (command_str.find("Delete pool: ") == 0){
            command = command_str.substr(13);
            std::string pool_name;
            position1 = command.find('(');
            if (position1 != 0){
                throw std::logic_error("Incorrect input: not enough ( or it is in the wrong place");
            }
            position2 = command.find(')');
            if (position2 == std::string::npos){
                throw std::logic_error("Incorrect input: not enough ) or it is in the wrong place");
            }
            pool_name = command.substr(position1+1, position2 - 1);
            if (pool_name.empty()){
                throw std::logic_error("Pool name not entered");
            }
            try{
                delete_pool(pool_name);
            }catch(const std::logic_error& er){
                throw er;
            }
            std::string message = "You have successfully delete pool: ";
            message += pool_name;
            if (_logger != nullptr) _logger->log(message, logger::severity::warning);

        }else if (command_str.find("Add scheme: ") == 0){
            command = command_str.substr(12);
            std::string pool_name;
            std::string scheme_name;
            try{
                pool_name = parsing_string_format(command, "pool name");
            }catch (const std::logic_error& er){
                throw er;
            }
            position1 = command.find('(');
            if (position1 != 0){
                throw std::logic_error("Incorrect input: not enough ( or it is in the wrong place");
            }
            position2 = command.find(')');
            if (position2 == std::string::npos){
                throw std::logic_error("Incorrect input: not enough ) or it is in the wrong place");
            }
            scheme_name = command.substr(position1+1, position2 - 1);
            if (pool_name.empty()){
                throw std::logic_error("Scheme name not entered");
            }
            try{
                add_scheme(pool_name, scheme_name);
            }catch (const std::logic_error& er){
                throw er;
            }
            std::string message = "You have successfully add scheme: ";
            message += scheme_name;
            message += " with pool: ";
            message += pool_name;
            if (_logger != nullptr) _logger->log(message, logger::severity::warning);

        }else if (command_str.find("Delete scheme: ") == 0){
            command = command_str.substr(15);
            std::string pool_name;
            std::string scheme_name;
            try{
                pool_name = parsing_string_format(command, "pool name");
            }catch (const std::logic_error& er){
                throw er;
            }
            position1 = command.find('(');
            if (position1 != 0){
                throw std::logic_error("Incorrect input: not enough ( or it is in the wrong place");
            }
            position2 = command.find(')');
            if (position2 == std::string::npos){
                throw std::logic_error("Incorrect input: not enough ) or it is in the wrong place");
            }
            scheme_name = command.substr(position1+1, position2 - 1);
            if (pool_name.empty()){
                throw std::logic_error("Scheme name not entered");
            }
            try{
                delete_scheme(pool_name, scheme_name);
            }catch (const std::logic_error& er){
                throw er;
            }
            std::string message = "You have successfully delete scheme: ";
            message += scheme_name;
            message += " with pool: ";
            message += pool_name;
            if (_logger != nullptr) _logger->log(message, logger::severity::warning);

        }else if (command_str.find("Add collection: ") == 0){
            command = command_str.substr(16);
            std::string pool_name;
            std::string scheme_name;
            std::string collection_name;
            try{
                pool_name = parsing_string_format(command, "pool name");
                scheme_name = parsing_string_format(command, "scheme name");
            }catch (const std::logic_error& er){
                throw er;
            }
            position1 = command.find('(');
            if (position1 != 0){
                throw std::logic_error("Incorrect input: not enough ( or it is in the wrong place");
            }
            position2 = command.find(')');
            if (position2 == std::string::npos){
                throw std::logic_error("Incorrect input: not enough ) or it is in the wrong place");
            }
            collection_name = command.substr(position1+1, position2 - 1);
            if (pool_name.empty()){
                throw std::logic_error("Scheme name not entered");
            }
            try{
                add_collection(pool_name, scheme_name, collection_name);
            }catch (const std::logic_error& er){
                throw er;
            }
            std::string message = "You have successfully add collection: ";
            message += collection_name;
            message += " with scheme: ";
            message += scheme_name;
            message += " with pool: ";
            message += pool_name;
            if (_logger != nullptr) _logger->log(message, logger::severity::warning);

        }else if (command_str.find("Delete collection: ") == 0){
            command = command_str.substr(19);
            std::string pool_name;
            std::string scheme_name;
            std::string collection_name;
            try{
                parsing_full_map(pool_name, scheme_name, collection_name, command);
                delete_collection(pool_name, scheme_name, collection_name);
            }catch (const std::logic_error& er){
                throw er;
            }

            std::string message = "You have successfully delete collection: ";
            message += collection_name;
            message += " with scheme: ";
            message += scheme_name;
            message += " with pool: ";
            message += pool_name;
            if (_logger != nullptr) _logger->log(message, logger::severity::warning);

        }else if (command_str.find("Insert: ") == 0){
            command = command_str.substr(8);
            std::string pool_name;
            std::string scheme_name;
            std::string collection_name;
            try{
                parsing_full_map(pool_name, scheme_name, collection_name, command);
            }catch(const std::logic_error& er){
                throw er;
            }
            session_key key;

            try{
                key = std::move(parsing_session_key(key_str));
                session_value value = std::move(paring_session_value(value_str, key));
                insert(key, value, pool_name, scheme_name, collection_name);
                std::string message = "You have successfully insert: with collection: ";
                message += collection_name;
                message += ", with scheme: ";
                message += scheme_name;
                message += ", with pool: ";
                message += pool_name;
                message += '\n';
                message += session_key_to_string(key);
                message += '\n';
                message += session_value_to_string(value);
                if (_logger != nullptr) _logger->log(message, logger::severity::warning);
            }catch (const std::logic_error& er){
                throw er;
            }

        }else if (command_str.find("Remove: ") == 0){
            command = command_str.substr(8);
            std::string pool_name;
            std::string scheme_name;
            std::string collection_name;
            try{
                parsing_full_map(pool_name, scheme_name, collection_name, command);
            }catch(const std::logic_error& er){
                throw er;
            }
            session_key key;
            try{
                key = std::move(parsing_session_key(key_str));
                remove(key, pool_name, scheme_name, collection_name);
            }catch (const std::logic_error& er){
                throw er;
            }
            std::string message = "You have successfully remove: with collection: ";
            message += collection_name;
            message += ", with scheme: ";
            message += scheme_name;
            message += ", with pool: ";
            message += pool_name;
            message += "\n";
            message += session_key_to_string(key);
            if (_logger != nullptr) _logger->log(message, logger::severity::warning);
        }else if (command_str.find("Get key information: ") == 0){
            command = command_str.substr(21);
            std::string pool_name;
            std::string scheme_name;
            std::string collection_name;
            try{
                parsing_full_map(pool_name, scheme_name, collection_name, command);
            }catch(const std::logic_error& er){
                throw er;
            }
            session_key key;
            try{
                key = std::move(parsing_session_key(key_str));
                session_value value = std::move(get_key_information(key, pool_name, scheme_name, collection_name));
                std::string message = "You have successfully get key information: with collection: ";
                message += collection_name;
                message += ", with scheme: ";
                message += scheme_name;
                message += ", with pool: ";
                message += pool_name;
                message += "\n";
                message += session_key_to_string(key);
                message += "\n";
                message += session_value_to_string(value);
                if (_logger != nullptr) _logger->log(message, logger::severity::warning);
            }catch (const std::logic_error& er){
                throw er;
            }

        }else if (command_str.find("Update key: ") == 0){
            command = command_str.substr(12);
            std::string pool_name;
            std::string scheme_name;
            std::string collection_name;
            try{
                parsing_full_map(pool_name, scheme_name, collection_name, command);
            }catch(const std::logic_error& er){
                throw er;
            }
            session_key key;
            try{
                key = std::move(parsing_session_key(key_str));
                session_value value = std::move(paring_session_value(value_str, key));
                update_key(key, value, pool_name, scheme_name, collection_name);
                std::string message = "You have successfully update key: with collection: ";
                message += collection_name;
                message += ", with scheme: ";
                message += scheme_name;
                message += ", with pool: ";
                message += pool_name;
                message += "\n";
                message += session_key_to_string(key);
                message += "\n";
                message += session_value_to_string(value);
                if (_logger != nullptr) _logger->log(message, logger::severity::warning);
            }catch (const std::logic_error& er){
                throw er;
            }


        }else if (command_str.find("Read range: ") == 0){
            command = command_str.substr(12);
            std::string pool_name;
            std::string scheme_name;
            std::string collection_name;
            try{
                parsing_full_map(pool_name, scheme_name, collection_name, command);
            }catch(const std::logic_error& er){
                throw er;
            }
            session_key key1;
            session_key* key1_ptr;
            session_key key2;
            session_key* key2_ptr;
            position1 = key_str.find('[');
            if (position1 != 0){
                throw std::logic_error("Incorrect input: not enough [ or it is in the wrong place");
            }

            position2 = key_str.find(']');
            if (position2 == std::string::npos){
                throw std::logic_error("Incorrect input: not enough ] or it is in the wrong place");
            }
            key_str = key_str.substr(position1 + 1, position2 -1);
            if (!key_str.empty()){
                try{
                    key1 = std::move(parsing_session_key(key_str));
                    key1_ptr = &key1;
                }catch (const std::logic_error& er){
                    throw er;
                }
            }else{
                key1_ptr = nullptr;
            }
            position1 = value_str.find('[');
            if (position1 != 0){
                throw std::logic_error("Incorrect input: not enough [ or it is in the wrong place");
            }

            position2 = value_str.find(']');
            if (position2 == std::string::npos){
                throw std::logic_error("Incorrect input: not enough ] or it is in the wrong place");
            }
            value_str = value_str.substr(position1 + 1, position2 -1);
            if (!value_str.empty()){
                try{
                    key2 = std::move(parsing_session_key(value_str));
                    key2_ptr = &key2;
                }catch (const std::logic_error& er){
                    throw er;
                }
            }else{
                key2_ptr = nullptr;
            }

            std::queue<std::tuple<session_key, session_value>> result;
            try{
               result = read_range(key1_ptr, key2_ptr, pool_name, scheme_name, collection_name);
            }catch (const std::logic_error& er){
                throw er;
            }
            std::string message;
            if (result.empty()){
                message = "There are no such data";
            }else{
                while (!result.empty()){

                    message += session_key_to_string(std::get<0>(result.front()));
                    message += session_value_to_string(std::get<1>(result.front()));
                    if (_logger != nullptr) _logger->log(message, logger::severity::warning);
                    message = ' ';
                    result.pop();
                }
            }
        }else if (command_str.find("Reset database: ") == 0){
            reset_database();
            if (_logger != nullptr) _logger->log("You have successfully reset database", logger::severity::warning);
        }else{
            throw std::logic_error("Incorrect command format");
        }
    }

    void parsing_in_file(const std::string& path){
        if (_logger != nullptr) _logger->log("------------The parsing in file has started working-----------", logger::severity::warning);
        std::ifstream file;
        file.open(path);
        if (!file.is_open()){
            throw std::logic_error("Incorrect path to file");
        }
        std::string command_line;
        std::string key_line;
        std::string value_line;
        while (std::getline(file, command_line)){
            if (command_line.find("Insert: ") == 0 || command_line.find("Update key: ") == 0 || command_line.find("Read range: ") == 0){
                if (!std::getline(file, key_line)){
                    file.close();
                    throw std::logic_error("Incorrect key");
                }
                if (!std::getline(file, value_line)){
                    file.close();
                    throw std::logic_error("Incorrect value");
                }
            }else if (command_line.find("Update key: ") == 0 || command_line.find("Get key information: ") == 0  || command_line.find("Remove: ") == 0){
                if (!std::getline(file, key_line)){
                    file.close();
                    throw std::logic_error("Incorrect key");
                }
            }else if (command_line.find("Add pool: ") == 0 || command_line.find("Delete pool: ") == 0 || command_line.find("Add scheme: ") == 0 || command_line.find("Delete scheme: ") == 0 || command_line.find("Add collection: ") == 0 || command_line.find("Delete collection: ") == 0){
                try{
                    parsing_enter(command_line, key_line, value_line);
                }catch(const std::logic_error& er){
                    file.close();
                    throw er;
                }
                continue;
            }else{
                file.close();
                throw std::logic_error("Incorrect command");
            }
            try{
                parsing_enter(command_line, key_line, value_line);
            }catch (const std::logic_error& er){
                file.close();
                throw er;
            }

        }
        file.close();
        if (_logger != nullptr) _logger->log("------------The parsing in file has started working-----------", logger::severity::warning);
    }
};
#endif //LABS_4_SEM_DATABASE_H
