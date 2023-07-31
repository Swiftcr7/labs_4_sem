

#ifndef LABS_4_SEM_ASSOCIOTIVE_CONTEINER_H
#define LABS_4_SEM_ASSOCIOTIVE_CONTEINER_H
#include <utility>

template<
        typename tkey,
        typename tvalue>
class associative_container
{

public:
    enum class bypass_detour{
        prefix,
        postfix,
        infix
    };

    struct key_value_pair
    {
        tkey _key;
        tvalue _value;
    };

public:

    virtual void insert(
            tkey const &key,
            const tvalue &value) = 0;

    void operator+=(
            key_value_pair pair);

    virtual bool find(
            key_value_pair *target_key_and_result_value) = 0;

    bool operator[](
            key_value_pair *target_key_and_result_value);

    virtual tvalue remove(
            tkey const &key) = 0;

//    virtual tvalue get(
//            key_value_pair *target_key_and_result_value) = 0;


    tvalue operator-=(
            key_value_pair pair);

    tvalue operator-=(
            tkey const &key);

    virtual void bypass_tree(bypass_detour detour) const = 0;

    virtual ~associative_container() = default;

};

template<
        typename tkey,
        typename tvalue>
void associative_container<tkey, tvalue>::operator+=(
        key_value_pair pair)
{
    return insert(pair._key, std::move(pair._value));
}

template<
        typename tkey,
        typename tvalue>
bool associative_container<tkey, tvalue>::operator[](
        key_value_pair *target_key_and_result_value)
{
    return find(target_key_and_result_value);
}

template<
        typename tkey,
        typename tvalue>
tvalue associative_container<tkey, tvalue>::operator-=(
        key_value_pair pair)
{
    return remove(pair._key);
}

template<
        typename tkey,
        typename tvalue>
tvalue associative_container<tkey, tvalue>::operator-=(
        tkey const &key)
{
    return remove(key);
}

#endif //LABS_4_SEM_ASSOCIOTIVE_CONTEINER_H
