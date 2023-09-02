#ifndef LABS_4_SEM_AVL_H
#define LABS_4_SEM_AVL_H

#include "../../labs_4_sem///binary_search_tree/binary_search_tree.h"
#include "print_AVL.h"

template<typename tkey, typename tvalue, typename tkey_comparer>
class AVL_tree: public binary_search_tree<tkey, tvalue, tkey_comparer>{
protected:

    struct node_AVL: public  binary_search_tree<tkey, tvalue, tkey_comparer>::node{
        node_AVL() = default;

        node_AVL(const tkey& key, const tvalue& value, node_AVL* left, node_AVL* right, size_t h) : binary_search_tree<tkey, tvalue, tkey_comparer>::node(key, value, left, right){
            height = h;
        }

        size_t height;

        virtual ~node_AVL() = default;
    };

protected:

//    void debug_print(void* root) const override {
//        debug_avl_tree_printing<tkey, tvalue>(reinterpret_cast<void*>(root));
//    }


    class AVL_insert_class final : public  binary_search_tree<tkey, tvalue, tkey_comparer>::insert_class{
    public:
        explicit AVL_insert_class(AVL_tree<tkey, tvalue, tkey_comparer> *tree) :
        binary_search_tree<tkey, tvalue, tkey_comparer>::insert_class(tree)
        {

        }
    private:
        AVL_tree<tkey, tvalue, tkey_comparer>* _tree;
    private:
        size_t get_height(node_AVL* current_node) const{
            if (current_node == nullptr){
                return 0;
            }else{
                return current_node->height;
            }
        }

        void set_height(node_AVL* current_node) const{
            auto* a = reinterpret_cast<node_AVL*>(current_node->left_node);
            auto* b = reinterpret_cast<node_AVL*>(current_node->right_node);
            current_node->height = std::max(get_height(a), get_height(b)) + 1;
        }

        int balance_difference(node_AVL* current_node) const{
            return (int)get_height(reinterpret_cast<node_AVL*>(current_node->left_node)) - (int)get_height(reinterpret_cast<node_AVL*>(current_node->right_node));
        }

    private:
        size_t get_node_size() const override{
            return sizeof (node_AVL);
        }

        void get_node_constructor(typename binary_search_tree<tkey, tvalue, tkey_comparer>::node** x, const tkey& key, const tvalue& value) const override{
            new (*(reinterpret_cast<node_AVL**>(x))) node_AVL{key, value, nullptr, nullptr, 1};
        }

    public:
        void insert_after(const tkey& key, const tvalue& value, typename binary_search_tree<tkey, tvalue, tkey_comparer>::node** root, std::stack<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node*>* way_to_insert, logger* logger) override
        {
            auto** AVL_root = reinterpret_cast<node_AVL**>(root);
            auto* AVL_way_to_insert = reinterpret_cast<std::stack<node_AVL*>*>(way_to_insert);
            insert_after_inner(AVL_root, AVL_way_to_insert, logger);
            if (logger != nullptr){
                logger->log("Balafncing is successful", logger::severity::debug);
            }
        }
    private:
        void insert_after_inner(node_AVL** root, std::stack<node_AVL*>* way_to_insert, logger* logger) const{
            node_AVL* current_node = way_to_insert->top();
            way_to_insert->pop();
            typename binary_search_tree<tkey, tvalue, tkey_comparer>::node** dad;
            if (!way_to_insert->empty()){
                if (reinterpret_cast<node_AVL*>(way_to_insert->top()->left_node) == current_node){
                    dad = reinterpret_cast<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node**>(&(way_to_insert->top()->left_node));
                }else{
                    dad = reinterpret_cast<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node**>(&(way_to_insert->top()->right_node));
                }
            }else{
                dad = reinterpret_cast<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node**>(root);
            }
            if (balance_difference(current_node) > 1){
                auto* a = reinterpret_cast<node_AVL*>(current_node->left_node);
                auto* b = current_node;
                if (balance_difference(a) < 0){
                    auto* c = reinterpret_cast<node_AVL*>(a->right_node);
                    auto* d = a;
                    _tree->left_mini_rotate(reinterpret_cast<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node**>(&(current_node->left_node)));
                    if(logger != nullptr) logger->log("left mini rotate", logger::severity::debug);
                    set_height(d);
                    set_height(c);
                }
                _tree->right_mini_rotate(dad);
                if (logger != nullptr) logger->log("right mini rotate", logger::severity::debug);
                set_height(b);
                set_height(a);
                if (logger != nullptr) logger->log("Internal balancing", logger::severity::debug);
                if (!way_to_insert->empty()){
                    insert_after_inner(root, way_to_insert, logger);
                }

            }else if(balance_difference(current_node) < -1){
                auto* a = reinterpret_cast<node_AVL*>(current_node->right_node);
                auto* b = current_node;
                if (balance_difference(a) > 0){
                    auto* c = reinterpret_cast<node_AVL*>(a->left_node);
                    auto* d = a;
                    _tree->right_mini_rotate(reinterpret_cast<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node**>(&(current_node->right_node)));
                    if(logger!= nullptr) logger->log("right mini rotate", logger::severity::debug);
                    set_height(d);
                    set_height(c);
                    //a = reinterpret_cast<node_AVL*>(current_node->right_node);//TODO checking without it
                }
                _tree->left_mini_rotate(dad);
                if(logger != nullptr) logger->log("left mini rotate", logger::severity::debug);
                set_height(b);
                set_height(a);
                if (logger != nullptr) logger->log("Internal balancing", logger::severity::debug);
                if (!way_to_insert->empty()){
                    insert_after_inner(root, way_to_insert, logger);
                }
            }else {
                set_height(current_node);
                if (!way_to_insert->empty()){
                    insert_after_inner(root, way_to_insert, logger);
                }
            }
        }
    };
public:

    class AVL_remove_class : public binary_search_tree<tkey, tvalue, tkey_comparer>::remove_class{
    private:
        AVL_tree<tkey, tvalue, tkey_comparer>* _tree;
    private:
        int get_height(node_AVL* current_node) const{
            if (current_node == nullptr){
                return 0;
            }
            return current_node->height;
        }

        void set_height(node_AVL* current_node) const{
            auto a = reinterpret_cast<node_AVL*>(current_node->left_node);
            auto b = reinterpret_cast<node_AVL*>(current_node->right_node);
            current_node->height = std::max(get_height(a), get_height(b)) + 1;
        }

        int balance_difference(node_AVL* current_node) const{
            auto* a = reinterpret_cast<node_AVL*>(current_node->left_node);
            auto* b = reinterpret_cast<node_AVL*>(current_node->right_node);
            return (int)get_height(a) - (int)get_height(b);
        }


        void node_destructor(typename binary_search_tree<tkey, tvalue, tkey_comparer>::node* node1) const override{
            auto* node2 = reinterpret_cast<node_AVL*>(node1);
            node2->~node_AVL();
        }

        void remove_after_inner(node_AVL** root, std::stack<node_AVL*>* way_to_remove, logger* logger) {
            if (way_to_remove->empty()){
                if (*root != nullptr){
                    set_height(*root);
                }
                return;
            }
            node_AVL* current_node = way_to_remove->top();
            way_to_remove->pop();
            typename binary_search_tree<tkey, tvalue, tkey_comparer>::node** dad;
            if (!way_to_remove->empty()){
                if (reinterpret_cast<node_AVL*>(way_to_remove->top()->left_node) == current_node){
                    dad = reinterpret_cast<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node**>(&way_to_remove->top()->left_node);
                }else{
                    dad = reinterpret_cast<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node**>(&way_to_remove->top()->right_node);
                }
            }else{
                dad = reinterpret_cast<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node**>(root);
            }
            if (balance_difference(current_node) > 1){
                auto* a = reinterpret_cast<node_AVL*>(current_node->left_node);
                auto* b = current_node;
                if (balance_difference(a) < 0){
                    auto* c = reinterpret_cast<node_AVL*>(a->right_node);
                    auto* d = a;
                    _tree->left_mini_rotate(reinterpret_cast<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node**>(&(current_node->left_node)));
                    if(logger != nullptr) logger->log("left mini rotate", logger::severity::debug);
                    set_height(d);
                    set_height(c);
                    //a = reinterpret_cast<node_AVL*>(current_node->left_node);
                }
                _tree->right_mini_rotate(dad);
                if (logger != nullptr) logger->log("right mini rotate", logger::severity::debug);
                set_height(b);
                set_height(a);
                if (logger != nullptr) logger->log("Internal balancing", logger::severity::debug);
                if (!way_to_remove->empty()){
                    remove_after_inner(root, way_to_remove, logger);
                }

            }else if(balance_difference(current_node) < -1){
                auto* a = reinterpret_cast<node_AVL*>(current_node->right_node);
                auto* b = current_node;
                if (balance_difference(a) > 0){
                    auto* c = reinterpret_cast<node_AVL*>(a->left_node);
                    auto* d = a;
                    _tree->right_mini_rotate(reinterpret_cast<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node**>(&(current_node->right_node)));
                    if (logger != nullptr) logger->log("right mini rotate", logger::severity::debug);
                    set_height(d);
                    set_height(c);
                    //a = reinterpret_cast<node_AVL*>(current_node->right_node);//TODO checking without it
                }
                _tree->left_mini_rotate(dad);
                if (logger != nullptr) logger->log("left mini rotate", logger::severity::debug);
                set_height(b);
                set_height(a);
                if (logger != nullptr) logger->log("Internal balancing", logger::severity::debug);
                if (!way_to_remove->empty()){
                    remove_after_inner(root, way_to_remove, logger);
                }
            }else {
                set_height(current_node);
                if (!way_to_remove->empty()){
                    remove_after_inner(root, way_to_remove, logger);
                }
            }
        }
    public:
        explicit AVL_remove_class(AVL_tree<tkey, tvalue, tkey_comparer>* tree) :
        binary_search_tree<tkey, tvalue, tkey_comparer>::remove_class(tree){}

        void remove_after(tkey const& key, typename binary_search_tree<tkey, tvalue, tkey_comparer>::node** root, std::stack<typename binary_search_tree<tkey, tvalue, tkey_comparer>::node*>* way_to_remove, logger* logger) override{
            auto** AVL_root = reinterpret_cast<node_AVL**>(root);
            auto* AVL_way_to_remove = reinterpret_cast<std::stack<node_AVL*>*>(way_to_remove);
            remove_after_inner(AVL_root, AVL_way_to_remove, logger);
            if (logger != nullptr){
                logger->log("Balancing is successful", logger::severity::debug);
            }
        }
    };
private:
    size_t node_size() const override{
        return sizeof (node_AVL);
    }

    void node_construct(typename binary_search_tree<tkey, tvalue, tkey_comparer>::node* current_node_copy, typename binary_search_tree<tkey, tvalue, tkey_comparer>::node* current_node) const override{
        auto* AVL_current_node_copy = reinterpret_cast<node_AVL*>(current_node_copy);
        auto* AVL_current_node = reinterpret_cast<node_AVL*>(current_node);
        new (AVL_current_node_copy) node_AVL {AVL_current_node->key, AVL_current_node->value, reinterpret_cast<node_AVL*>(this->copy_inner(AVL_current_node->left_node)),reinterpret_cast<node_AVL*>(this->copy_inner(AVL_current_node->right_node)), AVL_current_node->height};
    }

    void additional_data(typename binary_search_tree<tkey, tvalue, tkey_comparer>::node* current_node_copy,typename binary_search_tree<tkey, tvalue, tkey_comparer>::node* current_node) const override{
        auto* AVL_current_node = reinterpret_cast<node_AVL*>(current_node);
        auto* AVL_current_node_copy = reinterpret_cast<node_AVL*>(current_node_copy);
        AVL_current_node_copy->height = AVL_current_node->height;
    }
public:

    explicit AVL_tree(memory* alloc_tree = nullptr, logger* log_tree = nullptr):
            binary_search_tree<tkey, tvalue, tkey_comparer>(alloc_tree, log_tree, nullptr, new typename binary_search_tree<tkey, tvalue, tkey_comparer>::find_class() , new AVL_insert_class(this), new AVL_remove_class(this))
    {
//
    }

    AVL_tree(const AVL_tree<tkey, tvalue, tkey_comparer>& other_tree ){
        this->_allocator = other_tree._allocator;
        this->_logger = other_tree._logger;
        this->_class_remove = new AVL_remove_class(this);
        this->_class_find = new typename binary_search_tree<tkey, tvalue, tkey_comparer>::_class_find;
        this->_class_insert = new AVL_insert_class(this);
        this->_root = other_tree.copy();
    }

    AVL_tree(const AVL_tree<tkey, tvalue, tkey_comparer>&& other_tree){
        this->_allocator = other_tree._allocator;
        this->_logger = other_tree._logger;
        this->_class_remove = new AVL_remove_class(this);
        this->_class_find = new typename binary_search_tree<tkey, tvalue, tkey_comparer>::_class_find;
        this->_class_insert = new AVL_insert_class(this);
        delete other_tree._class_find;
        delete other_tree._class_insert;
        delete other_tree._class_remove;
        this->_root = other_tree._root;
        other_tree._logger = nullptr;
        other_tree._allocator = nullptr;
        other_tree._class_find = nullptr;
        other_tree._class_insert = nullptr;
        other_tree._class_remove = nullptr;
    }

    AVL_tree<tkey, tvalue, tkey_comparer>& operator=(const AVL_tree<tkey, tvalue, tkey_comparer>& other_tree){
        if (*this != other_tree){
            this->clear();
            this->_root = other_tree.cop();
            return *this;
        }

        return *this;
    }

    AVL_tree<tkey, tvalue, tkey_comparer>& operator=(const AVL_tree<tkey, tvalue, tkey_comparer>&& other_tree){
        if (*this != other_tree){
            this->clear();
            this->_root = other_tree._root;
            this->_allocator = other_tree._allocator;
            this->_logger = other_tree._logger;
            delete other_tree._class_remove;
            delete other_tree._class_insert;
            delete other_tree._class_find;
            other_tree._logger = nullptr;
            other_tree._allocator = nullptr;
            other_tree._root = nullptr;
            other_tree._class_find = nullptr;
            other_tree._class_insert = nullptr;
            other_tree._class_remove = nullptr;
        }
        return *this;
    }

    ~AVL_tree(){

    }
};
#endif //LABS_4_SEM_AVL_H
