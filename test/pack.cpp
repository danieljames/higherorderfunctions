#include <boost/fit/pack.hpp>
#include <boost/fit/always.hpp>
#include <boost/fit/identity.hpp>
#include <memory>
#include "test.hpp"

BOOST_FIT_TEST_CASE()
{
    auto p1 = boost::fit::pack_basic(1, 2);
    auto p2 = p1;
    BOOST_FIT_TEST_CHECK(p2(binary_class()) == p1(binary_class()));
    
    BOOST_FIT_STATIC_TEST_CHECK(boost::fit::pack_basic(1, 2)(binary_class()) == 3);
    BOOST_FIT_TEST_CHECK(boost::fit::pack_basic(1, 2)(binary_class()) == 3 );

    BOOST_FIT_STATIC_TEST_CHECK(boost::fit::pack(1, 2)(binary_class()) == 3);
    BOOST_FIT_TEST_CHECK(boost::fit::pack(1, 2)(binary_class()) == 3 );

    BOOST_FIT_STATIC_TEST_CHECK(boost::fit::pack_forward(1, 2)(binary_class()) == 3);
    BOOST_FIT_TEST_CHECK(boost::fit::pack_forward(1, 2)(binary_class()) == 3 );
}
#if BOOST_FIT_HAS_NOEXCEPT_DEDUCTION
struct copy_throws 
{
    copy_throws() {}
    copy_throws(copy_throws const&) {}
    copy_throws(copy_throws&&) noexcept {}
};

BOOST_FIT_TEST_CASE()
{
    int i = 1;
    copy_throws ct{};
    static_assert(!noexcept(boost::fit::pack(ct, ct)(boost::fit::always())), "noexcept pack");
    static_assert(noexcept(boost::fit::pack(1, 2)(boost::fit::always())), "noexcept pack");
    static_assert(noexcept(boost::fit::pack_forward(ct, ct)(boost::fit::always())), "noexcept pack");
    static_assert(noexcept(boost::fit::pack_forward(i, i)(boost::fit::always())), "noexcept pack");
    static_assert(noexcept(boost::fit::pack_forward(1, 2)(boost::fit::always())), "noexcept pack");
    static_assert(noexcept(boost::fit::pack_basic(ct, ct)(boost::fit::always())), "noexcept pack");
    static_assert(noexcept(boost::fit::pack_basic(i, i)(boost::fit::always())), "noexcept pack");
    static_assert(noexcept(boost::fit::pack_basic(1, 2)(boost::fit::always())), "noexcept pack");

    static_assert(noexcept(boost::fit::pack()(boost::fit::always())), "noexcept pack");
    static_assert(noexcept(boost::fit::pack_forward()(boost::fit::always())), "noexcept pack");
    static_assert(noexcept(boost::fit::pack_basic()(boost::fit::always())), "noexcept pack");
}

BOOST_FIT_TEST_CASE()
{
    copy_throws ct{};
    static_assert(!noexcept(boost::fit::pack_join(boost::fit::pack(ct), boost::fit::pack(ct))(boost::fit::always())), "noexcept pack");
    static_assert(noexcept(boost::fit::pack_join(boost::fit::pack(1), boost::fit::pack(1))(boost::fit::always())), "noexcept pack");
    static_assert(noexcept(boost::fit::pack_join(boost::fit::pack(), boost::fit::pack())(boost::fit::always())), "noexcept pack");
    auto p = boost::fit::pack(1);
    static_assert(noexcept(boost::fit::pack_join(p, boost::fit::pack())(boost::fit::always())), "noexcept pack");
    static_assert(noexcept(boost::fit::pack_join(boost::fit::pack(), p)(boost::fit::always())), "noexcept pack");
    static_assert(noexcept(boost::fit::pack_join(p, p)(boost::fit::always())), "noexcept pack");
    auto pt = boost::fit::pack(ct);
    static_assert(!noexcept(boost::fit::pack_join(pt, boost::fit::pack())(boost::fit::always())), "noexcept pack");
    static_assert(!noexcept(boost::fit::pack_join(boost::fit::pack(), pt)(boost::fit::always())), "noexcept pack");
    static_assert(!noexcept(boost::fit::pack_join(pt, pt)(boost::fit::always())), "noexcept pack");

}
#endif
BOOST_FIT_TEST_CASE()
{
    static constexpr int x = 1;
    static constexpr int y = 2;

    auto p1 = boost::fit::pack_basic(x, y);
    static_assert(!boost::fit::detail::is_default_constructible<decltype(p1)>::value, "Pack default constructible");
    
    BOOST_FIT_STATIC_TEST_CHECK(boost::fit::pack_basic(x, y)(binary_class()) == 3);
    BOOST_FIT_TEST_CHECK(boost::fit::pack_basic(x, y)(binary_class()) == 3 );

    auto p2 = boost::fit::pack(std::ref(x), std::ref(y));
    static_assert(!boost::fit::detail::is_default_constructible<decltype(p2)>::value, "Pack default constructible");

    BOOST_FIT_STATIC_TEST_CHECK(boost::fit::pack(x, y)(binary_class()) == 3);
    BOOST_FIT_TEST_CHECK(boost::fit::pack(std::ref(x), std::ref(y))(binary_class()) == 3 );

    auto p3 = boost::fit::pack_forward(x, y);
    static_assert(!boost::fit::detail::is_default_constructible<decltype(p3)>::value, "Pack default constructible");

    BOOST_FIT_STATIC_TEST_CHECK(boost::fit::pack_forward(x, y)(binary_class()) == 3);
    BOOST_FIT_TEST_CHECK(boost::fit::pack_forward(x, y)(binary_class()) == 3 );
}

BOOST_FIT_TEST_CASE()
{
    BOOST_FIT_STATIC_TEST_CHECK(boost::fit::pack_basic()(boost::fit::always(3)) == 3);
    BOOST_FIT_TEST_CHECK(boost::fit::pack_basic()(boost::fit::always(3)) == 3 );
}

BOOST_FIT_TEST_CASE()
{
    BOOST_FIT_STATIC_TEST_CHECK(boost::fit::pack_basic(3)(boost::fit::identity) == 3);
    BOOST_FIT_TEST_CHECK(boost::fit::pack_basic(3)(boost::fit::identity) == 3 );
}

BOOST_FIT_TEST_CASE()
{
    auto p = boost::fit::pack(1);
    BOOST_FIT_TEST_CHECK(boost::fit::pack_join(p, boost::fit::pack(2))(binary_class()) == 3);
    BOOST_FIT_TEST_CHECK(boost::fit::pack_join(p, p)(binary_class()) == 2);
    BOOST_FIT_TEST_CHECK(boost::fit::pack_join(p, p, boost::fit::pack())(binary_class()) == 2);
    
}

BOOST_FIT_TEST_CASE()
{
    BOOST_FIT_STATIC_TEST_CHECK(boost::fit::pack_join(boost::fit::pack_basic(1), boost::fit::pack_basic(2))(binary_class()) == 3);
    BOOST_FIT_TEST_CHECK(boost::fit::pack_join(boost::fit::pack_basic(1), boost::fit::pack_basic(2))(binary_class()) == 3 );

    BOOST_FIT_STATIC_TEST_CHECK(boost::fit::pack_join(boost::fit::pack(1), boost::fit::pack(2))(binary_class()) == 3);
    BOOST_FIT_TEST_CHECK(boost::fit::pack_join(boost::fit::pack(1), boost::fit::pack(2))(binary_class()) == 3 );

    BOOST_FIT_STATIC_TEST_CHECK(boost::fit::pack_join(boost::fit::pack_forward(1), boost::fit::pack_forward(2))(binary_class()) == 3);
    BOOST_FIT_TEST_CHECK(boost::fit::pack_join(boost::fit::pack_forward(1), boost::fit::pack_forward(2))(binary_class()) == 3 );
}

BOOST_FIT_TEST_CASE()
{
    BOOST_FIT_STATIC_TEST_CHECK(boost::fit::pack_join(boost::fit::pack_basic(), boost::fit::pack_basic(1, 2))(binary_class()) == 3);
    BOOST_FIT_TEST_CHECK(boost::fit::pack_join(boost::fit::pack_basic(), boost::fit::pack_basic(1, 2))(binary_class()) == 3 );

    BOOST_FIT_STATIC_TEST_CHECK(boost::fit::pack_join(boost::fit::pack(), boost::fit::pack(1, 2))(binary_class()) == 3);
    BOOST_FIT_TEST_CHECK(boost::fit::pack_join(boost::fit::pack(), boost::fit::pack(1, 2))(binary_class()) == 3 );

    BOOST_FIT_STATIC_TEST_CHECK(boost::fit::pack_join(boost::fit::pack_forward(), boost::fit::pack_forward(1, 2))(binary_class()) == 3);
    BOOST_FIT_TEST_CHECK(boost::fit::pack_join(boost::fit::pack_forward(), boost::fit::pack_forward(1, 2))(binary_class()) == 3 );
}

BOOST_FIT_TEST_CASE()
{
    BOOST_FIT_STATIC_TEST_CHECK(boost::fit::pack_join(boost::fit::pack_basic(1, 2), boost::fit::pack_basic())(binary_class()) == 3);
    BOOST_FIT_TEST_CHECK(boost::fit::pack_join(boost::fit::pack_basic(1, 2), boost::fit::pack_basic())(binary_class()) == 3 );

    BOOST_FIT_STATIC_TEST_CHECK(boost::fit::pack_join(boost::fit::pack(1, 2), boost::fit::pack())(binary_class()) == 3);
    BOOST_FIT_TEST_CHECK(boost::fit::pack_join(boost::fit::pack(1, 2), boost::fit::pack())(binary_class()) == 3 );

    BOOST_FIT_STATIC_TEST_CHECK(boost::fit::pack_join(boost::fit::pack_forward(1, 2), boost::fit::pack_forward())(binary_class()) == 3);
    BOOST_FIT_TEST_CHECK(boost::fit::pack_join(boost::fit::pack_forward(1, 2), boost::fit::pack_forward())(binary_class()) == 3 );
}

BOOST_FIT_TEST_CASE()
{
    BOOST_FIT_STATIC_TEST_CHECK(boost::fit::pack_join(boost::fit::pack_basic(1), boost::fit::pack_basic(), boost::fit::pack_basic(2))(binary_class()) == 3);
    BOOST_FIT_TEST_CHECK(boost::fit::pack_join(boost::fit::pack_basic(1), boost::fit::pack_basic(), boost::fit::pack_basic(2))(binary_class()) == 3 );

    BOOST_FIT_STATIC_TEST_CHECK(boost::fit::pack_join(boost::fit::pack(1), boost::fit::pack(), boost::fit::pack(2))(binary_class()) == 3);
    BOOST_FIT_TEST_CHECK(boost::fit::pack_join(boost::fit::pack(1), boost::fit::pack(), boost::fit::pack(2))(binary_class()) == 3 );

    BOOST_FIT_STATIC_TEST_CHECK(boost::fit::pack_join(boost::fit::pack_forward(1), boost::fit::pack_forward(), boost::fit::pack_forward(2))(binary_class()) == 3);
    BOOST_FIT_TEST_CHECK(boost::fit::pack_join(boost::fit::pack_forward(1), boost::fit::pack_forward(), boost::fit::pack_forward(2))(binary_class()) == 3 );
}

BOOST_FIT_TEST_CASE()
{
    BOOST_FIT_STATIC_TEST_CHECK(boost::fit::pack_join(boost::fit::pack_basic(), boost::fit::pack_basic(1), boost::fit::pack_basic(), boost::fit::pack_basic(2))(binary_class()) == 3);
    BOOST_FIT_TEST_CHECK(boost::fit::pack_join(boost::fit::pack_basic(), boost::fit::pack_basic(1), boost::fit::pack_basic(), boost::fit::pack_basic(2))(binary_class()) == 3 );

    BOOST_FIT_STATIC_TEST_CHECK(boost::fit::pack_join(boost::fit::pack(), boost::fit::pack(1), boost::fit::pack(), boost::fit::pack(2))(binary_class()) == 3);
    BOOST_FIT_TEST_CHECK(boost::fit::pack_join(boost::fit::pack(), boost::fit::pack(1), boost::fit::pack(), boost::fit::pack(2))(binary_class()) == 3 );

    BOOST_FIT_STATIC_TEST_CHECK(boost::fit::pack_join(boost::fit::pack_forward(), boost::fit::pack_forward(1), boost::fit::pack_forward(), boost::fit::pack_forward(2))(binary_class()) == 3);
    BOOST_FIT_TEST_CHECK(boost::fit::pack_join(boost::fit::pack_forward(), boost::fit::pack_forward(1), boost::fit::pack_forward(), boost::fit::pack_forward(2))(binary_class()) == 3 );
}

BOOST_FIT_TEST_CASE()
{
    BOOST_FIT_STATIC_TEST_CHECK(boost::fit::pack_join(boost::fit::pack_basic(1), boost::fit::pack_basic(), boost::fit::pack_basic(2), boost::fit::pack_basic())(binary_class()) == 3);
    BOOST_FIT_TEST_CHECK(boost::fit::pack_join(boost::fit::pack_basic(1), boost::fit::pack_basic(), boost::fit::pack_basic(2), boost::fit::pack_basic())(binary_class()) == 3 );

    BOOST_FIT_STATIC_TEST_CHECK(boost::fit::pack_join(boost::fit::pack(1), boost::fit::pack(), boost::fit::pack(2), boost::fit::pack())(binary_class()) == 3);
    BOOST_FIT_TEST_CHECK(boost::fit::pack_join(boost::fit::pack(1), boost::fit::pack(), boost::fit::pack(2), boost::fit::pack())(binary_class()) == 3 );

    BOOST_FIT_STATIC_TEST_CHECK(boost::fit::pack_join(boost::fit::pack_forward(1), boost::fit::pack_forward(), boost::fit::pack_forward(2), boost::fit::pack_forward())(binary_class()) == 3);
    BOOST_FIT_TEST_CHECK(boost::fit::pack_join(boost::fit::pack_forward(1), boost::fit::pack_forward(), boost::fit::pack_forward(2), boost::fit::pack_forward())(binary_class()) == 3 );
}

struct deref
{
    int operator()(const std::unique_ptr<int>& i) const
    {
        return *i;
    }
};

BOOST_FIT_TEST_CASE()
{
    std::unique_ptr<int> i(new int(3));
    BOOST_FIT_TEST_CHECK(boost::fit::pack_basic(i)(deref()) == 3);
    BOOST_FIT_TEST_CHECK(boost::fit::pack_basic(std::unique_ptr<int>(new int(3)))(deref()) == 3);
    BOOST_FIT_TEST_CHECK(boost::fit::pack_forward(std::unique_ptr<int>(new int(3)))(deref()) == 3);
    BOOST_FIT_TEST_CHECK(boost::fit::pack(std::unique_ptr<int>(new int(3)))(deref()) == 3);
    auto p = boost::fit::pack_basic(std::unique_ptr<int>(new int(3)));
    BOOST_FIT_TEST_CHECK(p(deref()) == 3);

    BOOST_FIT_TEST_CHECK(boost::fit::pack_join(boost::fit::pack_basic(), boost::fit::pack_basic(std::unique_ptr<int>(new int(3))))(deref()) == 3);
    BOOST_FIT_TEST_CHECK(boost::fit::pack_join(boost::fit::pack_forward(), boost::fit::pack_forward(std::unique_ptr<int>(new int(3))))(deref()) == 3);
    BOOST_FIT_TEST_CHECK(boost::fit::pack_join(boost::fit::pack(), boost::fit::pack(std::unique_ptr<int>(new int(3))))(deref()) == 3);
    // BOOST_FIT_TEST_CHECK(p(deref()) == 3);
}

struct move_rvalue 
{
    void operator()(std::string&& s) const 
    {
        std::string ss = std::move(s);
        BOOST_FIT_TEST_CHECK(ss == "abcdef");
        s = "00000";
    }
};

struct check_rvalue 
{
    void operator()(std::string&& s) const 
    {
        BOOST_FIT_TEST_CHECK(s == "abcdef");
    }
};

BOOST_FIT_TEST_CASE()
{
    auto p = boost::fit::pack_basic(std::string{"abcdef"});
    p(move_rvalue{});
    p(check_rvalue{});
}

BOOST_FIT_TEST_CASE()
{
    auto p = boost::fit::pack(std::string{"abcdef"});
    p(move_rvalue{});
    p(check_rvalue{});
}

struct empty1
{};

struct empty2
{};

BOOST_FIT_TEST_CASE()
{
    static_assert(boost::fit::detail::is_default_constructible<empty1, empty2>::value, "Not default constructible");
}

BOOST_FIT_TEST_CASE()
{
    static constexpr auto p = boost::fit::pack_basic(empty1());
    BOOST_FIT_TEST_CHECK(p(boost::fit::always(0)) == 0);
    BOOST_FIT_STATIC_TEST_CHECK(p(boost::fit::always(0)) == 0);
#ifndef _MSC_VER
    static_assert(std::is_empty<decltype(p)>::value, "Pack not empty");
#endif
    static_assert(boost::fit::detail::is_default_constructible<decltype(p)>::value, "Not default constructible");

}

BOOST_FIT_TEST_CASE()
{
    static constexpr auto p = boost::fit::pack_basic(empty1(), empty2());
    BOOST_FIT_TEST_CHECK(p(boost::fit::always(0)) == 0);
    BOOST_FIT_STATIC_TEST_CHECK(p(boost::fit::always(0)) == 0);
#ifndef _MSC_VER
    static_assert(std::is_empty<decltype(p)>::value, "Pack not empty");
#endif
    static_assert(boost::fit::detail::is_default_constructible<decltype(p)>::value, "Not default constructible");
}

BOOST_FIT_TEST_CASE()
{
    static constexpr auto p = boost::fit::pack_basic(boost::fit::pack_basic(), boost::fit::pack_basic());
    BOOST_FIT_TEST_CHECK(p(boost::fit::always(0)) == 0);
    BOOST_FIT_STATIC_TEST_CHECK(p(boost::fit::always(0)) == 0);
#ifndef _MSC_VER
    static_assert(std::is_empty<decltype(p)>::value, "Pack not empty");
#endif
    static_assert(boost::fit::detail::is_default_constructible<decltype(p)>::value, "Not default constructible");
}

BOOST_FIT_TEST_CASE()
{
    static constexpr auto p = boost::fit::pack_basic(empty1(), empty2(), empty1());
    BOOST_FIT_TEST_CHECK(p(boost::fit::always(0)) == 0);
    BOOST_FIT_STATIC_TEST_CHECK(p(boost::fit::always(0)) == 0);
#ifndef _MSC_VER
    static_assert(std::is_empty<decltype(p)>::value, "Pack not empty");
#endif
    static_assert(boost::fit::detail::is_default_constructible<decltype(p)>::value, "Not default constructible");
}

BOOST_FIT_TEST_CASE()
{
    static constexpr auto p = boost::fit::pack_basic(empty1(), boost::fit::pack_basic(empty1(), empty2()));
    BOOST_FIT_TEST_CHECK(p(boost::fit::always(0)) == 0);
    BOOST_FIT_STATIC_TEST_CHECK(p(boost::fit::always(0)) == 0);
#ifndef _MSC_VER
    static_assert(std::is_empty<decltype(p)>::value, "Pack not empty");
#endif
    static_assert(boost::fit::detail::is_default_constructible<decltype(p)>::value, "Not default constructible");
}

BOOST_FIT_TEST_CASE()
{
    static constexpr auto p = boost::fit::pack_basic(boost::fit::pack_basic(), boost::fit::pack_basic(boost::fit::pack_basic()), empty1(), boost::fit::pack_basic(empty1(), empty2()));
    BOOST_FIT_TEST_CHECK(p(boost::fit::always(0)) == 0);
    BOOST_FIT_STATIC_TEST_CHECK(p(boost::fit::always(0)) == 0);
#ifndef _MSC_VER
    static_assert(std::is_empty<decltype(p)>::value, "Pack not empty");
#endif
    static_assert(boost::fit::detail::is_default_constructible<decltype(p)>::value, "Not default constructible");
}

struct not_default_constructible
{
    int i;
    constexpr not_default_constructible(int x) : i(x)
    {}
};

struct select_i
{
    template<class T>
    constexpr int operator()(T&& x) const
    {
        return x.i;
    } 

    template<class T, class U>
    constexpr int operator()(T&& x, U&& y) const
    {
        return x.i + y.i;
    } 

    template<class T, class U, class V>
    constexpr int operator()(T&& x, U&& y, V&& z) const
    {
        return x.i + y.i + z.i;
    } 
};

BOOST_FIT_TEST_CASE()
{
    static_assert(!boost::fit::detail::is_default_constructible<not_default_constructible>::value, "Default constructible");
    auto p = boost::fit::pack_basic(not_default_constructible(3));
    static_assert(!boost::fit::detail::is_default_constructible<decltype(p)>::value, "Pack default constructible");
    auto p1 = boost::fit::pack_forward(p);
    static_assert(!boost::fit::detail::is_default_constructible<decltype(p1)>::value, "Packs default constructible");
    auto p2 = boost::fit::pack_forward(p, p);
    static_assert(!boost::fit::detail::is_default_constructible<decltype(p2)>::value, "Packs default constructible");
    auto p3 = boost::fit::pack_forward(p, p, p);
    static_assert(!boost::fit::detail::is_default_constructible<decltype(p3)>::value, "Packs default constructible");
    BOOST_FIT_TEST_CHECK(p(select_i()) == 3);
    BOOST_FIT_STATIC_TEST_CHECK(boost::fit::pack_basic(not_default_constructible(3))(select_i()) == 3);
}

BOOST_FIT_TEST_CASE()
{
    static_assert(!boost::fit::detail::is_default_constructible<not_default_constructible>::value, "Default constructible");
    auto p = boost::fit::pack_basic(not_default_constructible(1), not_default_constructible(2));
    static_assert(!boost::fit::detail::is_default_constructible<decltype(p)>::value, "Pack default constructible");
    auto p1 = boost::fit::pack_forward(p);
    static_assert(!boost::fit::detail::is_default_constructible<decltype(p1)>::value, "Packs default constructible");
    auto p2 = boost::fit::pack_forward(p, p);
    static_assert(!boost::fit::detail::is_default_constructible<decltype(p2)>::value, "Packs default constructible");
    auto p3 = boost::fit::pack_forward(p, p, p);
    static_assert(!boost::fit::detail::is_default_constructible<decltype(p3)>::value, "Packs default constructible");
    BOOST_FIT_TEST_CHECK(p(select_i()) == 3);
    BOOST_FIT_STATIC_TEST_CHECK(boost::fit::pack_basic(not_default_constructible(1), not_default_constructible(2))(select_i()) == 3);
}

BOOST_FIT_TEST_CASE()
{
    static_assert(!boost::fit::detail::is_default_constructible<not_default_constructible>::value, "Default constructible");
    auto p = boost::fit::pack_basic(not_default_constructible(1), not_default_constructible(1), not_default_constructible(1));
    static_assert(!boost::fit::detail::is_default_constructible<decltype(p)>::value, "Pack default constructible");
    auto p1 = boost::fit::pack_forward(p);
    static_assert(!boost::fit::detail::is_default_constructible<decltype(p1)>::value, "Packs default constructible");
    auto p2 = boost::fit::pack_forward(p, p);
    static_assert(!boost::fit::detail::is_default_constructible<decltype(p2)>::value, "Packs default constructible");
    auto p3 = boost::fit::pack_forward(p, p, p);
    static_assert(!boost::fit::detail::is_default_constructible<decltype(p3)>::value, "Packs default constructible");
    BOOST_FIT_TEST_CHECK(p(select_i()) == 3);
    BOOST_FIT_STATIC_TEST_CHECK(boost::fit::pack_basic(not_default_constructible(1), not_default_constructible(1), not_default_constructible(1))(select_i()) == 3);
}


