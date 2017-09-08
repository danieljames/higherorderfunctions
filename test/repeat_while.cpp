#include <boost/fit/repeat_while.hpp>
#include <boost/fit/reveal.hpp>
#include "test.hpp"

// TODO: Test default construction, and static initialization

struct increment_constant
{
    template<class T>
    constexpr std::integral_constant<int, T::value + 1> operator()(T) const noexcept
    {
        return std::integral_constant<int, T::value + 1>();
    }
};

struct increment
{
    template<class T>
    constexpr T operator()(T x) const noexcept
    {
        return x + 1;
    }
};

struct not_6_constant
{
    template<class T>
    constexpr std::integral_constant<bool, (T::value != 6)> 
    operator()(T) const noexcept
    {
        return std::integral_constant<bool, (T::value != 6)>();
    }
};

struct not_6
{
    template<class T>
    constexpr bool operator()(T x) const noexcept
    {
        return x != 6;
    }
};

struct not_limit
{
    template<class T>
    constexpr bool operator()(T x) const
    {
        return x != (BOOST_FIT_RECURSIVE_CONSTEXPR_DEPTH+4);
    }
};

#if BOOST_FIT_HAS_NOEXCEPT_DEDUCTION
BOOST_FIT_TEST_CASE()
{
    static_assert(noexcept(boost::fit::repeat_while(not_6())(increment())(1)), "noexcept repeat_while");
    static_assert(noexcept(boost::fit::repeat_while(not_6_constant())(increment_constant())(std::integral_constant<int, 1>())), "noexcept repeat_while");
}
#endif

BOOST_FIT_TEST_CASE()
{
    static_assert
    (
        std::is_same<
            std::integral_constant<int, 6>, 
            decltype(boost::fit::repeat_while(not_6_constant())(increment_constant())(std::integral_constant<int, 1>()))
        >::value,
        "Error"
    );

    std::integral_constant<int, 6> x = boost::fit::repeat_while(not_6_constant())(increment_constant())(std::integral_constant<int, 1>());
    boost::fit::test::unused(x);
}

BOOST_FIT_TEST_CASE()
{
    BOOST_FIT_STATIC_TEST_CHECK(boost::fit::repeat_while(not_6())(increment())(1) == 6);
    BOOST_FIT_TEST_CHECK(boost::fit::repeat_while(not_6())(increment())(1) == 6);
    BOOST_FIT_TEST_CHECK(boost::fit::reveal(boost::fit::repeat_while(not_6())(increment()))(1) == 6);
}

BOOST_FIT_TEST_CASE()
{
    BOOST_FIT_TEST_CHECK(boost::fit::repeat_while(not_limit())(increment())(1) == BOOST_FIT_RECURSIVE_CONSTEXPR_DEPTH+4);
#if BOOST_FIT_HAS_RELAXED_CONSTEXPR
    BOOST_FIT_STATIC_TEST_CHECK(boost::fit::repeat_while(not_limit())(increment())(1) == BOOST_FIT_RECURSIVE_CONSTEXPR_DEPTH+4);
#endif
}
