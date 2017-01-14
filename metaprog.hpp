#ifndef METAPROG_HPP
#define METAPROG_HPP

#include <iterator>
#include <type_traits>
#include <utility>

// underlying type

template<typename Iterator>
struct iterator_underlying_type_impl
{
    using type = std::remove_reference_t<decltype(*std::declval<Iterator>())>;
};

template<typename Container>
struct iterator_underlying_type_impl<std::back_insert_iterator<Container>>
{
    using type = typename Container::value_type;
};

template<typename Container>
struct iterator_underlying_type_impl<std::insert_iterator<Container>>
{
    using type = typename Container::value_type;
};

template<typename Iterator>
using iterator_underlying_type = typename iterator_underlying_type_impl<Iterator>::type;

template<typename Range>
using range_underlying_type = iterator_underlying_type<decltype(std::declval<Range>().begin())>;

// is_pair

template<typename T>
struct is_pair_impl
{
    static const bool value = false;
};

template<typename T1, typename T2>
struct is_pair_impl<std::pair<T1,T2>>
{
    static const bool value = true;
};

template<typename T>
struct is_pair :
    is_pair_impl < std::remove_const_t < std::remove_reference_t < T > > >
{};


// get_pair_first

template<typename>
struct get_pair_first_impl;

template<typename First, typename Second>
struct get_pair_first_impl<std::pair<First, Second>>
{
    using type = First;
};

template<typename T>
using get_pair_first = typename get_pair_first_impl< std::remove_const_t < std::remove_reference_t < T > > >::type;

// get_pair_second

template<typename>
struct get_pair_second_impl;

template<typename First, typename Second>
struct get_pair_second_impl<std::pair<First, Second>>
{
    using type = Second;
};

template<typename T>
using get_pair_second = typename get_pair_second_impl< std::remove_const_t < std::remove_reference_t < T > > >::type;

// does both contain versions from left AND right

template<bool isBothOutputAPair, typename BothOutputIterator, typename LeftRange, typename RightRange>
struct BothContainsLeftAndRightImpl
{
    static const bool value = false;
};

template<typename BothOutputIterator, typename LeftRange, typename RightRange>
struct BothContainsLeftAndRightImpl<true, BothOutputIterator, LeftRange, RightRange>
{
    static const bool value = 
        std::is_same
        <
            std::remove_const_t < get_pair_first < iterator_underlying_type < BothOutputIterator > > >,
            std::remove_const_t < range_underlying_type < LeftRange > >
        >::value
        &&
        std::is_same
        <
            std::remove_const_t < get_pair_second < iterator_underlying_type < BothOutputIterator > > >,
            std::remove_const_t < range_underlying_type < RightRange > >
        >::value;

};

template<typename BothOutputIterator, typename LeftRange, typename RightRange>
struct BothContainsLeftAndRight :
    BothContainsLeftAndRightImpl < is_pair < iterator_underlying_type < BothOutputIterator > >::value, BothOutputIterator, LeftRange, RightRange>
{};

#endif

