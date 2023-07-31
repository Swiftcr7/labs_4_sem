

#ifndef LABS_4_SEM_BINARY_SEARCH_TREE_H
#define LABS_4_SEM_BINARY_SEARCH_TREE_H



#include <iostream>
#include <stack>
#include "../../scc//allocator2.h"
#include "associotive_conteiner.h"
#include <functional>
#include <vector>
#include <ctime>
#include "print_BST.h"

template<typename  tkey, typename  tvalue, typename tkey_comparer>
class binary_search_tree : public associative_container<tkey, tvalue> {
protected:
    struct node{
        node() = default;
        node(tkey const &k, tvalue const &v, node* l, node* r):
                key(k), value(v), left_node(l), right_node(r){}
        tkey key;
        tvalue value;
        node* left_node;
        node* right_node;

        virtual ~node() = default;
    };

    tkey_comparer _comparator;
    node *_root;
    memory *_allocator;
    logger *_logger;


private:

    /////////////class prefix//////////////////////

    class prefix_iterator final{
    private:
        node* _current_node;
        binary_search_tree<tkey, tvalue, tkey_comparer>* _tree;
        std::stack<node*> _way;
        node* _rightest_node;
    public:
        prefix_iterator(node* current_node, binary_search_tree<tkey, tvalue, tkey_comparer>* tree) {
            _tree = tree;
            node* rightest_node = _tree->_root;
            if (rightest_node != nullptr) {
                while (rightest_node->right_node != nullptr) {
                    rightest_node = rightest_node->right_node;
                }
            }
            _rightest_node=rightest_node;
            if (current_node != nullptr) {
                _current_node = current_node;
                node *ptr = _tree->_root;
                while (ptr != current_node) {
                    _way.push(ptr);
                    if (_tree->_comparator(current_node->key, ptr->key) > 0) {
                        ptr = ptr->right_node;
                    } else {
                        ptr = ptr->left_node;
                    }
                }
            } else {
                _current_node = nullptr;
                _way = std::stack<node *>();
            }
        }
    public:
        bool operator==(prefix_iterator const &other) const{
            if (_way==other._way && _current_node==other._current_node){
                return true;
            }
            return false;

        }

        bool operator!=(prefix_iterator const &other) const{
            return !(*this==other);
        }

        prefix_iterator& operator++(){
            if(_current_node ==_rightest_node ){
                _current_node= nullptr;
                _way=std::stack<node*>();
                return *this;

            }
            if(_current_node->left_node!= nullptr) {
                _way.push(_current_node);
                _current_node = _current_node->left_node;
                return *this;
            }
            if ( _current_node->left_node== nullptr && _current_node->right_node!= nullptr){
                _way.push(_current_node);
                _current_node = _current_node->right_node;
                return *this;
            }

            if (_current_node->left_node== nullptr && _current_node->right_node== nullptr) {
                while(!_way.empty() && (_way.top()->right_node == _current_node || _way.top()->right_node == nullptr)){
                    while (!_way.empty() && _way.top()->right_node == _current_node) {
                        _current_node = _way.top();
                        _way.pop();
                    }

                    while (!_way.empty() && _way.top()->right_node == nullptr) {
                        _current_node = _way.top();
                        _way.pop();
                    }
                }
                _current_node=_way.top()->right_node;
                return *this;
            }

        }

        prefix_iterator operator++(int bred){
            prefix_iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        std::tuple<unsigned int, tkey const&, tvalue const&> operator*() const{
            return std::tuple<unsigned int, tkey const&, tvalue const&>(_way.size(),_current_node->key, _current_node->value);
        }
    };

    /////////////class infix//////////////////////

    class infix_iterator final {
    private:
        node* _current_node;
        binary_search_tree<tkey, tvalue, tkey_comparer>* _tree;
        node* _rightest_node;
        std::stack<node*> _way;
    public:
        infix_iterator(node* current_node, binary_search_tree<tkey, tvalue, tkey_comparer>* tree){
            _tree=tree;
            //_current_node=current_node;
            node* rightest_node=tree->_root;
            if (rightest_node!= nullptr){
                while(rightest_node->right_node!= nullptr){
                    rightest_node=rightest_node->right_node;
                }
            }
            _rightest_node=rightest_node;

            if (current_node!= nullptr){
                node* ptr=_tree->_root;
//                tkey_comparer functor;
                _current_node=current_node;
                while (ptr!=current_node){
                    _way.push(ptr);
                    if (_tree->_comparator(current_node->key, ptr->key)>0){
                        ptr=ptr->right_node;
                    }else{
                        ptr=ptr->left_node;
                    }
                }
            }else{

                _current_node= nullptr;
                _way=std::stack<node*>();
            }
        }
        bool operator==(infix_iterator const& other) const{
            if (_way==other._way && _current_node==other._current_node){
                return true;
            }else{
                return false;
            }
        }
        bool operator!=(infix_iterator const& other) const{
            return !(*this==other);
        }
        infix_iterator& operator++(){
            if (_current_node == _rightest_node){
                _current_node= nullptr;
                _way=std::stack<node*>();
                return *this;
            }
            if (_current_node->right_node!= nullptr){
                _way.push(_current_node);
                _current_node=_current_node->right_node;
                while(_current_node->left_node!= nullptr){
                    _way.push(_current_node);
                    _current_node=_current_node->left_node;
                }
            }else {
                if (!_way.empty()) {
                    if (_way.top()->left_node == _current_node) {
                        _current_node = _way.top();
                        _way.pop();
                    } else {
                        while (!_way.empty() && _way.top()->right_node == _current_node) {
                            _current_node = _way.top();
                            _way.pop();
                        }
                        _current_node = _way.top();
                        _way.pop();
                    }
                }
            }
            return *this;
        }
        infix_iterator operator++(int bred){
            infix_iterator current=*this;
            ++(*this);
            return current;
        }

        std::tuple<unsigned int, tkey const&, tvalue const&> operator*() const{
            return std::tuple<unsigned int, tkey const&, tvalue const&>(_way.size(), _current_node->key, _current_node->value);
        }
    };

    /////////////class postfix//////////////////////

    class postfix_iterator final{
        friend class binary_search_tree<tkey, tvalue, tkey_comparer>;
    private:
        node* _current_node;
        node* _root_node;
        std::stack<node *> _way;
        binary_search_tree<tkey, tvalue, tkey_comparer>* _tree;
    public:
        postfix_iterator(node* current_node, binary_search_tree<tkey, tvalue, tkey_comparer>* tree){
            _tree=tree;
            _root_node=_tree->_root;
            if (current_node!= nullptr){
                node* ptr=_tree->_root;
                _current_node=current_node;
                while(ptr!= current_node){
                    _way.push(ptr);
                    if (tree->_comparator(current_node->key, ptr->key)>0){

                        ptr=ptr->right_node;
                    }else{

                        ptr=ptr->left_node;
                    }
                }
            }else{
                _current_node = nullptr;
                _way=std::stack<node*>();
            }

        }
        bool operator==(postfix_iterator const& other){
            if(this->_way==other._way && this->_current_node==other._current_node){
                return true;
            }else{
                return false;
            }
        }
        bool operator!=(postfix_iterator const& other){
            return !(this->operator==(other));
        }
        postfix_iterator& operator++(){
            if (_current_node== nullptr){
                return *this;
            }
            if(_way.empty()){
                _current_node= nullptr;
                return *this;
            }
            if (_way.top()->right_node==_current_node){
                _current_node=_way.top();
                _way.pop();
            }else{
                if(_way.top()->right_node!= nullptr){
                    _current_node=_way.top()->right_node;
                    while(_current_node->right_node!= nullptr || _current_node->left_node!= nullptr){
                        _way.push(_current_node);
                        if(_current_node->left_node!=nullptr){
                            _current_node=_current_node->left_node;
                        }else{
                            _current_node=_current_node->right_node;
                        }
                    }
                }else{
                    _current_node=_way.top();
                    _way.pop();
                }
            }
            return *this;
        }
        postfix_iterator operator++(int bred){
            postfix_iterator other=*this;
            this->operator++();
            return other;
        }
        std::tuple<unsigned int, tkey, tvalue> operator*(){
            return std::tuple<unsigned int, tkey, tvalue>(_way.size(), this->_current_node->key, this->_current_node->value);
        }

        node* get_node(){
            return _current_node;
        }
    };

    /////////////begin/end prefix//////////////////////

    prefix_iterator prefix_it_begin()const noexcept{
        return prefix_iterator(_root, const_cast<binary_search_tree<tkey, tvalue, tkey_comparer>*>(this));
    }

    prefix_iterator prefix_it_end()const noexcept{
        return prefix_iterator(nullptr, const_cast<binary_search_tree<tkey, tvalue, tkey_comparer>*>(this));
    }

    /////////////begin/end infix//////////////////////

    infix_iterator infix_it_begin()const noexcept{
        node* ptr=_root;
        if(ptr!= nullptr){
            while (ptr->left_node!= nullptr){
                ptr=ptr->left_node;
            }
        }
        return infix_iterator(ptr, const_cast<binary_search_tree<tkey, tvalue, tkey_comparer>*>(this));
    }

    infix_iterator infix_it_end()const noexcept{
        return infix_iterator(nullptr, const_cast<binary_search_tree<tkey, tvalue, tkey_comparer>*>(this));
    }

    /////////////begin/end postfix//////////////////////



    postfix_iterator postfix_it_begin()const noexcept{
        node* ptr = _root;
        if (_root == nullptr){
            return postfix_it_end();
        }
        if(_root->left_node != nullptr){
            while(ptr->left_node != nullptr ){
                ptr=ptr->left_node;
            }
        }else{
            while(ptr->left_node== nullptr && !(ptr->left_node== nullptr && ptr->right_node== nullptr)){
                ptr = ptr->right_node;
            }
            while(ptr->left_node!=nullptr){
                ptr = ptr->left_node;
            }
        }
        return postfix_iterator(ptr, const_cast<binary_search_tree<tkey, tvalue, tkey_comparer>*>(this));
    }
    postfix_iterator postfix_it_end()const noexcept{
        return postfix_iterator(nullptr, const_cast<binary_search_tree<tkey, tvalue, tkey_comparer>*>(this));
    }

    /////////////class find_class//////////////////////
protected:
    class find_class{
    protected:
        virtual void after_find(typename associative_container<tkey, tvalue>::key_value_pair* key_value_to_find, node*& current_node, std::stack<node**> way_to_find_node){

        }

    protected:
        bool find_inner(typename associative_container<tkey, tvalue>::key_value_pair* key_value_to_find, node*& current_node, std::stack<node**> way_to_find_node){
            if(current_node == nullptr){
                return false;
            }
            tkey_comparer comparer;
            int res=comparer(current_node->key, key_value_to_find->_key);
            bool res_find=false;
            if(res == 0){
                return true;
            }
            if (current_node->right_node== nullptr && current_node->left_node== nullptr){
                return false;
            }
            node* ptr=current_node;
            if (res>0){
                ptr=ptr->left_node;
            }else{
                ptr=ptr->right_node;
            }
            way_to_find_node.push(&ptr);
            res_find=find_inner(key_value_to_find,  ptr, way_to_find_node);
            way_to_find_node.pop();
            after_find(key_value_to_find,  current_node, way_to_find_node);
            return res_find;


        }

    public:
        bool find(typename associative_container<tkey, tvalue>::key_value_pair* key_value_to_find, node*& current_node){
            std::stack<node**> way_to_find_node;
            return find_inner(key_value_to_find,  current_node, way_to_find_node);
        }

        virtual ~find_class() = default;
    };


    /////////////class get_class//////////////////////

//    class get_class{
//    protected:
//        virtual void after_get(associative_container<tkey, tvalue>::key_value_pair* key_value_to_get, node*& current_node, std::stack<node**> way_to_get_node){
//
//        }
//
//    private:
//        tvalue get_inner(associative_container<tkey, tvalue>::key_value_pair* key_value_to_get, node*& current_node, std::stack<node**> way_to_get_node){
//            if(current_node == nullptr){
//                throw std::logic_error("cjedjc ");
//            }
//            tkey_comparer comparer;
//            int res=comparer(current_node->key, key_value_to_get->_key);
//            tvalue res_get;
//            if(res == 0){
//                return key_value_to_get->_value;
//            }
//            if (current_node->right_node== nullptr && current_node->left_node== nullptr){
//                throw std::logic_error("cfccecef");
//            }
//            node* ptr=current_node;
//            if (res>0){
//                ptr=ptr->left_node;
//            }else{
//                ptr=ptr->right_node;
//            }
//            way_to_get_node.push(&ptr);
//            res_get = get_inner(key_value_to_get,  ptr, way_to_get_node);
//            way_to_get_node.pop();
//            after_get(key_value_to_get,  current_node, way_to_get_node);
//            return res_get;
//
//
//        }
//
//    public:
//        tvalue get(associative_container<tkey, tvalue>::key_value_pair* key_value_to_get, node*& current_node){
//            std::stack<node**> way_to_get_node;
//            return get_inner(key_value_to_get,  current_node, way_to_get_node);
//        }
//
//        virtual ~get_class() = default;
//    };


protected:

    ////////////////////class insert_class//////////////////////////

    class insert_class{
    public:
        explicit insert_class(binary_search_tree<tkey, tvalue, tkey_comparer>* this_bst):
        _tree(this_bst){}
    private:
        binary_search_tree<tkey, tvalue, tkey_comparer>* _tree;
    protected:

        virtual size_t get_node_size() const {
            return reinterpret_cast<size_t>(sizeof(node));
        }

        virtual void get_node_constructor(node** x, const tkey& key, const tvalue& value) const {
            new (*x) node{key, value, nullptr, nullptr};
        }
//    public:
//        void insert(const tkey& key,const tvalue& value){
//            std::stack<node *> way_to_insert = std::stack<node*>();
//            return insert_inner(key, std::forward<tvalue>(value),_tree->_root, way_to_insert);
//
//        }

    public:
        void insert_inner(const tkey& key, const tvalue& value, node** root, std::stack<node*>* way_to_insert, memory* allocator, logger* logger){
            node* current_node = *root;
            int flag;
            tkey_comparer comparator = tkey_comparer();
            if (*root == nullptr){
                *root = reinterpret_cast<node*>(allocator->allocate(get_node_size()));
                get_node_constructor(root, key, value);
                way_to_insert->push(*root);
                if (logger != nullptr){
                    logger->log("New node ", logger::severity::debug);
                }
                return;
            }
            while (current_node != nullptr){
                int comparer_result = comparator(key, current_node->key);
                way_to_insert->push(current_node);
                if (comparer_result == 0){
                    throw std::logic_error("Identical keys");
                }else if (comparer_result > 0){
                    flag = 1;
                    current_node = current_node->right_node;
                }else{
                    flag = 0;

                    current_node = current_node->left_node;
                }
            }
            node** tmp;
            if (flag == 1){
                tmp =&way_to_insert->top()->right_node;
            }else{
                tmp = &way_to_insert->top()->left_node;
            }
            (*tmp) = reinterpret_cast<node*>(allocator->allocate(get_node_size()));
            get_node_constructor(tmp, key, value);
            way_to_insert->push(*tmp);
            if (logger != nullptr){
                logger->log("New node " , logger::severity::debug);
            }
        }
    public:
        virtual void insert_after(const tkey& key, const tvalue& value, node** root, std::stack<node*>* way_to_insert, logger* logger){
            while(way_to_insert->empty()) way_to_insert->pop();

        }
    public:
        virtual ~insert_class() = default;
    };


    ////////////////////class remove_class//////////////////////////

protected:
    class remove_class{
    private:
        binary_search_tree<tkey, tvalue, tkey_comparer>* _tree;
    public:
        explicit remove_class(binary_search_tree<tkey, tvalue, tkey_comparer> *tree) :
            _tree(tree){}
    protected:
        virtual void node_destructor(node* node1) const{
            node1->~node();
        }
//        tvalue remove(tkey const &key, node*& current_node){
//            std::stack <node**> way_to_remove;
//            return remove_inner(key, current_node, way_to_remove);
//        }
    public:
        tvalue remove_inner(tkey const &key, node** root, std::stack<node*>* way_to_remove, memory* allocator, logger* logger){
            node* current_node = *root;
            int flag = 2;
            tkey_comparer comparator = tkey_comparer();
            while (current_node != nullptr && comparator(key, current_node->key) != 0){
                if (comparator(key, current_node->key) > 0){
                    way_to_remove->push(current_node);
                    current_node = current_node->right_node;
                    flag = 1;
                }else{
                    way_to_remove->push(current_node);
                    current_node = current_node->left_node;
                    flag = 0;
                }
            }
            if (current_node == nullptr){
                throw std::logic_error("There is no such key in the tree");
            }

            if (current_node->left_node == nullptr && current_node->right_node == nullptr){
                if (flag == 1){
                    way_to_remove->top()->right_node = nullptr;
                }else if(flag == 0){
                    way_to_remove->top()->left_node = nullptr;
                }
                tvalue removing_value = std::move(current_node->value);
                node_destructor(current_node);
                allocator->deallocate(reinterpret_cast<void*>(current_node));
                if (flag == 2){
                    *root == nullptr;
                }
                if (logger != nullptr){
                    logger->log("Node successfully deleted", logger::severity::debug);
                }
                return removing_value;
            }else if ((current_node->left_node == nullptr && current_node->right_node != nullptr) || (current_node->left_node != nullptr && current_node->right_node == nullptr)){
                if (current_node->left_node == nullptr && current_node->right_node != nullptr){
                    if (flag == 2){
                        *root = current_node->right_node;
                    }else if (flag == 0){
                        way_to_remove->top()->left_node = current_node->right_node;
                    }else{
                        way_to_remove->top()->right_node = current_node->right_node;
                    }
                }else{
                    if (flag == 2){
                        *root = current_node->left_node;
                    }else if (flag == 0){
                        way_to_remove->top()->left_node = current_node->left_node;
                    }else{
                        way_to_remove->top()->right_node = current_node->left_node;
                    }
                }
                tvalue removing_value = std::move(current_node->value);
                node_destructor(current_node);
                allocator->deallocate(reinterpret_cast<void*>(current_node));
                if (logger != nullptr){
                    logger->log("Node successfully deleted", logger::severity::debug);
                }
                return removing_value;
            }else if (current_node->left_node != nullptr && current_node->right_node != nullptr){
                node* replace_node = current_node;
                way_to_remove->push(current_node);
                replace_node = replace_node->right_node;
                if (replace_node->left_node == nullptr){
                    tvalue removing_value = std::move(current_node->value);
                    current_node->key = replace_node->key;
                    current_node->value = replace_node->value;
                    current_node->right_node = replace_node->right_node;
                    node_destructor(current_node);
                    allocator->deallocate(reinterpret_cast<void*>(replace_node));
                    if (logger != nullptr){
                        logger->log("Node successfully deleted", logger::severity::debug);
                    }
                    return removing_value;
                }else{
                    while (replace_node->left_node != nullptr){
                        way_to_remove->push(replace_node);
                        replace_node = replace_node->left_node;
                    }
                    tvalue removing_value = std::move(current_node->value);
                    current_node->key = replace_node->key;
                    current_node->value = replace_node->value;
                    way_to_remove->top()->left_node = replace_node->right_node;
                    node_destructor(current_node );
                    allocator->deallocate(reinterpret_cast<void*>(replace_node));
                    if (logger != nullptr){
                        logger->log("Node successfully deleted", logger::severity::debug);
                    }
                    return removing_value;
                }
            }
        }

        virtual void remove_after(tkey const& key, node** root, std::stack<node*>* way_to_remove, logger* logger){
            while (!way_to_remove->empty()){
                way_to_remove->pop();
            }

        }
    public:
        virtual ~remove_class() = default;


    };




protected:
    find_class* _class_find;
    insert_class* _class_insert;
    remove_class* _class_remove;
//    get_class* _class_get;


public:
    bool find(typename associative_container<tkey, tvalue>::key_value_pair* key_value_to_find)override{
        return _class_find->find(key_value_to_find,_root);
    }

    void insert( tkey const &key, const tvalue& value) override{
        std::stack<node *> insert_way = std::stack<node*>();
        node** current_root = &_root;
        _class_insert->insert_inner(key, value, current_root, &insert_way, this->_allocator, this->_logger);
        _class_insert->insert_after(key, value, current_root, &insert_way, this->_logger);

    }

    tvalue remove(tkey const &key) override{
        node** current_root = &_root;
        std::stack<node*> way_to_remove = std::stack<node*>();
        tvalue remove_value = _class_remove->remove_inner(key,current_root,  &way_to_remove, _allocator, _logger);
        _class_remove->remove_after(key, current_root, &way_to_remove, _logger);
        return remove_value;
    }

//    tvalue get(associative_container<tkey, tvalue>::key_value_pair* key_value_to_get)override{
//        return _class_get->get(key_value_to_get, _root);
//    }

    ////REMOVE AFTER CHECKING !!
protected:

    binary_search_tree(memory* alloc_tree, logger* log_tree, insert_class* insert):
            _root(nullptr), _logger(log_tree), _allocator(alloc_tree), _class_insert(insert)
    {
        _class_find = new find_class();
        _class_remove = new remove_class(this);
//        _class_get = new get_class();
    }
protected:
    void clear(){
        clear_inner(_root);

    }

    void clear_inner(node* current_node){
        if (current_node == nullptr){
            return;
        }

        auto* left = current_node->left_node;
        auto* right = current_node->right_node;
        current_node->~node();//TODO Get_node_destructor ?
        _allocator->deallocate(current_node);
        clear_inner(left);
        clear_inner(right);

    }

    node* copy() const{
        copy_inner(_root);

    }

    node* copy_inner(node* current_node) const{
        if (current_node == nullptr){
            return nullptr;
        }
        auto* new_node = reinterpret_cast<node*>(_allocator->allocate(node_size()));
        node_construct(new_node, current_node);
        additional_data(new_node, current_node);
    }

    virtual size_t node_size() const{
        return sizeof(node);
    }

    virtual void node_construct(node* current_node_copy, node* current_node) const{
        new (current_node_copy) node {current_node->key, current_node->value, copy_inner(current_node->left_node),copy_inner(current_node->right_node)};
    }

    virtual void additional_data(node* current_node_copy, node* current_node) const{}

public:
    /////////////tree constructor//////////////////
    binary_search_tree(memory* alloc_tree, logger* log_tree){
        _root = nullptr;
        _logger = log_tree;
        _allocator = alloc_tree;
        //_comparator;
        _class_find = new find_class();
        _class_insert = new insert_class(this);
        _class_remove = new remove_class(this);
//        _class_get = new get_class();
    }

    /////////////tree constructor2//////////////////
    binary_search_tree(memory* alloc_tree, logger* log_tree, node* root, find_class* find, insert_class* insert, remove_class* remove){
        _root = root;
        _logger = log_tree;
        _allocator = alloc_tree;
        //_comparator;
        _class_find = find;
        _class_insert = insert;
        _class_remove = remove;
//        _class_get = get;
    }

    /////////////tree destructor//////////////////
    ~binary_search_tree(){

        auto end_it=postfix_it_end();
        for (auto it=postfix_it_begin(); it != end_it; it++){
            _allocator->deallocate(it.get_node());
        }
        delete _class_find;
        delete _class_find;
        delete _class_insert;
        delete _class_remove;
//        delete _class_get;
    }


    /////////////tree copy constructor//////////////////
    void copy_nodes(node* node){
        if (node != nullptr){
            _class_insert->insert(node->key, node->value);
            copy_nodes(node->left_node);
            copy_nodes(node->right_node);
        }
    }

    binary_search_tree(const binary_search_tree<tkey, tvalue, tkey_comparer> &other_tree){
        _root = nullptr;
        _allocator = other_tree._allocator;
        _logger = other_tree._logger;
        _comparator = other_tree._comparator;
        _class_find = other_tree._class_find;
        _class_insert = other_tree._class_insert;
        _class_remove = other_tree._class_remove;
//        _class_get = other_tree._class_get;
        copy_nodes(other_tree._root);
    }
    binary_search_tree<tkey, tvalue, tkey_comparer>& operator=(const binary_search_tree<tkey, tvalue, tkey_comparer> &other_tree){
        if (other_tree == *this){
            return *this;
        }
        this->clear();
        _root = other_tree.copy();
        return *this;
    }
    binary_search_tree(binary_search_tree<tkey, tvalue, tkey_comparer>&& other_tree){
        _allocator = other_tree._allocator;
        _logger = other_tree._logger;
        _comparator = std::move(other_tree._comparator);///??
        _class_find = other_tree._class_find;
        _class_insert = other_tree._class_insert;
        _class_remove = other_tree._class_remove;
//        _class_get=other_tree._class_get;
        other_tree._class_find= nullptr;
        other_tree._class_remove= nullptr;
        other_tree._class_insert= nullptr;
        other_tree._class_get= nullptr;
        _root=other_tree._root;
        other_tree._root= nullptr;
    }
    binary_search_tree<tkey, tvalue, tkey_comparer>& operator=(const binary_search_tree<tkey, tvalue, tkey_comparer>&& other_tree){
        if (*this != other_tree){

            this->clear();
            _root = other_tree._root;
            _allocator = other_tree._allocator;
            _logger = other_tree._logger;
            delete other_tree._class_insert;
            delete other_tree._class_remove;
            delete other_tree._class_find;
            other_tree._root = nullptr;
            other_tree._allocator = nullptr;
            other_tree._class_insert = nullptr;
            other_tree._logger = nullptr;
            other_tree._class_find = nullptr;
            other_tree._class_remove = nullptr;
        }


        return *this;
    }

protected:

    virtual void debug_print(void* root) const {
        debug_tree_printing<tkey, tvalue>(root);
    }

public:
    void bypass_tree(typename associative_container<tkey, tvalue>::bypass_detour detour )const override {
        if (detour == associative_container<tkey, tvalue>::bypass_detour::prefix) {

            for (auto it = prefix_it_begin(); it != prefix_it_end(); ++it) {

                std::cout << "key: " << std::get<1>(*it) << ", value: \"" << std::get<2>(*it) << "\"" << std::endl;
            }
        } else if (detour == associative_container<tkey, tvalue>::bypass_detour::postfix) {

            for (auto it = postfix_it_begin(); it != postfix_it_end(); it++) {


                std::cout << "key: " << std::get<1>(*it) << ", value: \"" << std::get<2>(*it) << "\"" << std::endl;
            }

        } else {
            for (auto it = infix_it_begin(); it != infix_it_end(); it++) {

                std::cout << "key: " << std::get<1>(*it) << ", value: \"" << std::get<2>(*it) << "\"" << std::endl;
            }
        }
        debug_print(_root);
    }
protected:
    void left_mini_rotate(node** current_node) const{
        node* right_subtree = (*current_node)->right_node;
        (*current_node)->right_node = right_subtree->left_node;
        right_subtree->left_node = (*current_node);
        (*current_node) = right_subtree;
    }

    void right_mini_rotate(node** current_node) const{
        node* left_subtree = (*current_node)->left_node;
        (*current_node)->left_node = left_subtree->right_node;
        left_subtree->right_node = (*current_node);
        (*current_node) = left_subtree;
    }
};
#endif //LABS_4_SEM_BINARY_SEARCH_TREE_H
