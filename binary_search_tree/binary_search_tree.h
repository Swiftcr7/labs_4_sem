//
// Created by HP on 16.07.2023.
//

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
        tkey key;
        tvalue value;
        node* right_node;
        node* left_node;
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

    class insert_class: public allocator{
    private:
        binary_search_tree<tkey, tvalue, tkey_comparer>* _tree;
    public:
        insert_class(binary_search_tree<tkey, tvalue, tkey_comparer>* tree){
            _tree=tree;
        }
        void insert(const tkey& key, tvalue&& value){
            std::stack<node **> way_to_insert;
            return insert_inner(key, std::forward<tvalue>(value),_tree->_root, way_to_insert);

        }
    private:
        void insert_inner(const tkey& key, tvalue&& value, node*& current_node, std::stack<node**> &way_to_insert){
            node* current_node1 = current_node;
            if (current_node != nullptr) {
                while (current_node1->left_node != nullptr || current_node1->right_node != nullptr) {
                    if (_tree->_comparator(key, current_node1->key) == 0) {
                        throw std::logic_error("two same keys---------------------------------------------------------------");
                        return;
                    }
                    if (_tree->_comparator(key, current_node1->key) > 0) {
                        if(current_node1->right_node != nullptr) {
                            current_node1 = current_node1->right_node;
                        }else{
                            break;
                        }
                    } else {
                        if (current_node1->left_node != nullptr) {
                            current_node1 = current_node1->left_node;
                        }else{
                            break;
                        }
                    }
                }
                if (_tree->_comparator(key, current_node1->key) == 0) {
                    throw std::logic_error("two same keys");
                    return;
                }
            }
            if (current_node == nullptr){
                current_node = reinterpret_cast<node*>(_tree->_allocator->allocate(sizeof(node)));
                new (current_node) node{key, std::forward<tvalue>(value), nullptr, nullptr};
                if (!way_to_insert.empty()){
                    if (_tree->_comparator(key, (*way_to_insert.top())->key) > 0){
                        (*way_to_insert.top())->right_node=current_node;
                    }else{
                        (*way_to_insert.top())->left_node=current_node;
                    }

                }
                _tree->_logger->log("Insert new node with key "+ std::to_string(key), logger::severity::information);
                insert_after(key, current_node, way_to_insert);
                return;
            }
            node* next_node;

            if (_tree->_comparator(current_node->key,key )>0){
                next_node=current_node->left_node;
            }else{
                next_node=current_node->right_node;
            }
            way_to_insert.push(&current_node);
            insert_inner(key, std::forward<tvalue>(value), next_node, way_to_insert);
            way_to_insert.pop();
            insert_after(key, current_node, way_to_insert);







        }
    protected:
        virtual void insert_after(tkey const &key, node*& current_node, std::stack<node **>& way_to_insert){

        }
    };


    ////////////////////class remove_class//////////////////////////

protected:
    class remove_class{
    private:
        binary_search_tree<tkey, tvalue, tkey_comparer>* _tree;
    public:
        remove_class(binary_search_tree<tkey, tvalue, tkey_comparer> *tree){
            _tree = tree;
        }
        tvalue remove(tkey const &key, node*& current_node){
            std::stack <node**> way_to_remove;
            return remove_inner(key, current_node, way_to_remove);
        }
    private:
        tvalue remove_inner(tkey const &key, node*& current_node, std::stack<node**>& way_to_remove){
            if (current_node == nullptr){
                throw std::invalid_argument("You want to delete nullptr-node");
            }
            if (_tree->_comparator(key, current_node->key)==0) {
                tvalue removing_value=current_node->value;

                if (current_node->left_node == nullptr && current_node->right_node == nullptr) {

                    if (!way_to_remove.empty()) {

                        if ((*way_to_remove.top())->right_node == current_node){
                            (*way_to_remove.top())->right_node = nullptr;
                        }else{
                            (*way_to_remove.top())->left_node = nullptr;
                        }

                    }
                    current_node->~node();
                    _tree->_allocator->deallocate(current_node);
                    current_node = nullptr;
                }
                else {
                    if ((current_node->right_node == nullptr && current_node->left_node != nullptr) ||
                        (current_node->right_node != nullptr && current_node->left_node == nullptr)) {
                        node *replaced_node =
                                current_node->left_node == nullptr ? current_node->right_node : current_node->left_node;
                        if (!way_to_remove.empty()) {
                            if ((*way_to_remove.top())->left_node == current_node) {
                                (*way_to_remove.top())->left_node = replaced_node;
                            } else {
                                (*way_to_remove.top())->right_node = replaced_node;
                            }
                        }
                        current_node->~node();
                        _tree->_allocator->deallocate(current_node);
                        current_node = replaced_node;
                    } else {
                        if (current_node->right_node != nullptr && current_node->left_node != nullptr) {
                            node *min_bin_node = current_node->right_node;

                            while (min_bin_node->left_node != nullptr)
                            {
                                min_bin_node = min_bin_node->left_node;
                            }

                            tkey key_current = min_bin_node->key;
                            tvalue value_current = min_bin_node->value;
                            _tree->remove(min_bin_node->key);

                            current_node->key = key_current;
                            current_node->value = value_current;
//                            node *max_node_in_left_subtree = current_node->left_node;
//
//                            while (max_node_in_left_subtree->right_node != nullptr) {
//                                max_node_in_left_subtree = max_node_in_left_subtree->right_node;
//                            }
//                            tkey key_of_max_node = max_node_in_left_subtree->key;
//                            tvalue value_of_max_node = max_node_in_left_subtree->value;

//                            _tree->remove(max_node_in_left_subtree->key);
//                            current_node->key = key_of_max_node;
//                            current_node->value = value_of_max_node;
                        }
                    }
                }
                return removing_value;

            }
            node* next_node = current_node;
            if (_tree->_comparator(key, current_node->key) > 0){
                next_node = current_node->right_node;
            }else{
                next_node = current_node->left_node;
            }
            way_to_remove.push(&current_node);
            tvalue removing_value = remove_inner(key, next_node, way_to_remove);
            way_to_remove.pop();
            remove_after(key,current_node, way_to_remove);

            return removing_value;

        }

        virtual void remove_after(tkey const& key, node*& current_node, std::stack<node**> &way_to_remove){

        }


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

    void insert( tkey const &key, tvalue&& value)override{
        return _class_insert->insert(key, std::forward<tvalue>(value));
    }

    tvalue remove(tkey const &key)override{
        return _class_remove->remove(key, _root);
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
        if(_root!= nullptr){
            postfix_iterator it;
            auto end_it=postfix_it_end();
            for (it=postfix_it_begin(); it != end_it; it++){///todo
                it.get_node()->~get_node();
                _allocator->deallocate(it.get_node());
            }
            _root= nullptr;
            this->copy_nodes(other_tree._root);
            return *this;
        }else{
            this->copy_nodes(other_tree._root);
            return *this;
        }
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
        if (*this==other_tree){
            return *this;
        }
        _allocator = other_tree._allocator;
        _logger = other_tree._logger;
        _comparator = std::move(other_tree._comparator);///??
        other_tree._class_insert = nullptr;
        other_tree._class_remove = nullptr;
        other_tree._class_find = nullptr;
        other_tree._class_get= nullptr;
        if(_root!= nullptr){
            _root= other_tree._root;
            postfix_iterator it;
            auto end_it=other_tree.postfix_it_end();
            for (it=other_tree.postfix_it_begin(); it != other_tree.end_it; it++){///???
                it.get_node()->~node();
                _allocator->deallocate(it.get_node());
            }
            return *this;
        }else{
            return *this;
        }
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
        debug_tree_printing<tkey, tvalue>(_root);
    }
protected:
    virtual void left_mini_rotate(node** current_node)const{
        node* right_subtree = (*current_node)->right_node;
        (*current_node)->right_node = right_subtree->left_node;
        right_subtree->left_node = (*current_node);
        *current_node = right_subtree;
    }

    virtual void right_mini_rotate(node** current_node)const{
        node* left_subtree = (*current_node)->left_node;
        (*current_node)->left_node = left_subtree->right_node;
        left_subtree->right_node = (*current_node);
        *current_node = left_subtree;
    }





};


#endif //LABS_4_SEM_BINARY_SEARCH_TREE_H
