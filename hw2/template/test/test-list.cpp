#include <sstream>
#include <string>

#include "catch2/catch_all.hpp"

#include "simulator-lib/mystd/list.h"

using namespace mystd;

template <typename T>
std::string stringify_list(const list<T> &l) {
    std::ostringstream oss;
    for (const auto &i: l)
        oss << i;
    return oss.str();
}

TEST_CASE("list - initialization") {
    list<int> fl;
    REQUIRE(fl.size() == 0);
}

TEST_CASE("list - insert") {
    list<int> fl;
    REQUIRE(fl.size() == 0);
    for (int i = 0; i < 10; ++i)
        fl.push_front(i);
    REQUIRE(fl.size() == 10);
    REQUIRE(stringify_list(fl) == "9876543210");
    fl.clear();

    REQUIRE(fl.size() == 0);

    fl.push_front(42);
    REQUIRE(fl.front() == 42);

    REQUIRE(*fl.insert(fl.begin(), 3) == 3);
    REQUIRE(*fl.insert(fl.begin(), 2) == 2);
    REQUIRE(*fl.insert(fl.begin(), 1) == 1);
    REQUIRE(fl.size() == 4);
    REQUIRE(stringify_list(fl) == "12342");

    fl.clear();
    for (int i = 0; i < 10; ++i)
        fl.insert(fl.begin(), i);
    REQUIRE(stringify_list(fl) == "9876543210");

    auto it = ++++++fl.begin();
    it = fl.insert(it, 999);
    REQUIRE(stringify_list(fl) == "9879996543210");
    ++++it;
    fl.insert(it, 7777);
    REQUIRE(stringify_list(fl) == "98799967777543210");
}

TEST_CASE("list - erase") {
    list<int> fl;
    REQUIRE(fl.size() == 0);
    for (int i = 0; i < 10; ++i)
        fl.push_back(i);
    REQUIRE(fl.size() == 10);
    REQUIRE(stringify_list(fl) == "0123456789");

    fl.pop_back();
    fl.pop_back();
    REQUIRE(fl.size() == 8);
    REQUIRE(stringify_list(fl) == "01234567");

    fl.pop_front();
    fl.pop_front();
    fl.pop_front();
    REQUIRE(fl.size() == 5);
    REQUIRE(stringify_list(fl) == "34567");

    unsigned i = 0;
    for (auto it = fl.begin(); it != fl.end(); )
        if (!(*it & 1))
            it = fl.erase(it);
        else ++it;
    REQUIRE(fl.size() == 3);
    REQUIRE(stringify_list(fl) == "357");
}

TEST_CASE("list - iterator") {
    std::ostringstream oss;
    list<int> fl;
    for (int i = 0; i < 10; ++i)
        fl.push_front(i);
    REQUIRE(stringify_list(fl) == "9876543210");
}

TEST_CASE("list - merge (ints)") {
    list<int> list1{1, 3, 5};
    list<int> list2{2, 4, 6};
    list1.merge(std::move(list2), std::less<>());
    REQUIRE(stringify_list(list1) == "123456");
    REQUIRE(list1.size() == 6);
    REQUIRE(list2.empty());
    REQUIRE(std::is_sorted(list1.begin(), list1.end()));
}

struct Person {
    std::string name;
    int age;

    bool operator<(const Person &rhs) const noexcept { return age < rhs.age; }
};

TEST_CASE("list - merge (persons)") {
    list<Person> list1{
            {"David", 20},
            {"Bob", 25},
            {"Alice", 30},
            {"Charlie", 35}
    };
    list<Person> list2{
            {"Pipin", 120},
            {"Sam", 253},
            {"Merry", 345},
            {"Frodo", 1230}
    };
    list1.merge(std::move(list2), std::less<>{});
    std::ostringstream oss;
    for (const auto &i : list1) oss << i.name << ' ';
    REQUIRE(oss.str() == "David Bob Alice Charlie Pipin Sam Merry Frodo ");
    REQUIRE(list1.size() == 8);
    REQUIRE(list2.empty());
    REQUIRE(std::is_sorted(list1.begin(), list1.end()));
}

TEST_CASE("list - sort (ints)") {
    list<int> list1{3, 1, 2};
    list1.sort(std::less<>{});
    REQUIRE(stringify_list(list1) == "123");
    REQUIRE(std::is_sorted(list1.begin(), list1.end()));
    REQUIRE(list1.size() == 3);

    list<int> list2{70, 81, -43, 20, -72, 38, -99, 58, -95, -16, 50, -67, -25, -19, 21, 30, 100, 95, -31, -76, -5, -24, -29, 97, 76, -27, -34, 71, -100, 8, 72, 0, 7, -1, 33, -12, 91, -15, -4, -49, -81, -60, 57, 23, -10, 13, -94, 90, 79, 4, 1, 52, 84, 46, -87, -93, -59, 34, -80, 62, 56, -92, 31, -8, 96, 73, -58, 44, -35, 25, 51, -38, 22, 40, 93, 24, 16, 36, 43, 78, 64, -42, 74, -54, 27, 54, 2, 9, 11, -85, -71, 3, 5, -62, -45, 29, -96, -74, -79, 75, -22, -73, 12, -78, -69, -26, 15, 68, 26, 32, 86, -83, -14, -91, -39, -55, 48, 61, -90, -32, -84, 39, 55, -44, -88, -30, -21, -64, -46, 18, -20, 82, 17, -68, 88, 85, 19, -86, 92, -17, 6, -98, -41, -47, -52, 60, 41, -77, 35, -82, -37, -36, -50, 42, -9, -23, -89, -56, 59, 87, 83, 65, -65, 67, 99, 80, 53, -63, -75, 14, -48, -28, 37, -57, 98, -51, -97, -13, 28, -6, -11, -2, 45, -70, 94, -40, 47, 77, -66, -7, -3, -61, 49, 66, -18, 89, -33, 69, -53, 63, 10};
    list2.sort(std::less<>{});
    REQUIRE(std::is_sorted(list2.begin(), list2.end()));
    REQUIRE(list2.size() == 201);
}

TEST_CASE("list - merge & sort (aggregates)") {
    list<Person> list1{
            {"Alice", 30},
            {"Bob", 25},
            {"Charlie", 35},
            {"David", 20}
    };
    list1.sort(std::less<>{});
    list<Person> list2{
            {"Frodo", 1230},
            {"Sam", 253},
            {"Merry", 345},
            {"Pipin", 120}
    };
    list2.sort(std::less<>{});

    list1.merge(std::move(list2), std::less<>{});
    REQUIRE(list1.size() == 8);
    REQUIRE(list2.empty());
    REQUIRE(std::is_sorted(list1.begin(), list1.end(), std::less<>{}));
}