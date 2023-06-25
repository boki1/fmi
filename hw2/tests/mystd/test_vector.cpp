#include <mystd/enable.h>

#ifdef FMI_OOP_USE_MYSTD_VECTOR

#include <gtest/gtest.h>

#include <sstream>
#include <iostream>

#include <mystd/vector.h>

using namespace mystd;

TEST(VectorTest, Construction) {
    vector<int> default_constructed_vec;
    EXPECT_EQ(default_constructed_vec.size(), 0);
    EXPECT_TRUE(default_constructed_vec.capacity() > 0);

    vector<int> vec_from_initializer_list{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    vector<int> vec_from_copy_ctor = vec_from_initializer_list;
    default_constructed_vec = vec_from_initializer_list;

    EXPECT_EQ(vec_from_copy_ctor.size(), 10);
    EXPECT_TRUE(vec_from_copy_ctor.capacity() >= 10);

    EXPECT_EQ(default_constructed_vec.size(), vec_from_initializer_list.size());
    EXPECT_EQ(default_constructed_vec.capacity(), vec_from_initializer_list.capacity());

    EXPECT_EQ(vec_from_copy_ctor.size(), vec_from_initializer_list.size());
    EXPECT_EQ(vec_from_copy_ctor.capacity(), vec_from_initializer_list.capacity());
}

TEST(VectorTest, PushBack) {
    vector<int> vec;
    const std::size_t default_cap = vec.capacity();
    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(3);
    EXPECT_EQ(vec.size(), 3);
    EXPECT_EQ(vec.capacity(), default_cap);
    EXPECT_EQ(vec[0], 1);
    EXPECT_EQ(vec[1], 2);
    EXPECT_EQ(vec[2], 3);
}

TEST(VectorTest, PopBack) {
    vector<int> v{1, 2, 3};
    const auto default_cap = v.capacity();
    v.pop_back();
    EXPECT_EQ(v.size(), 2);
    EXPECT_EQ(v.capacity(), default_cap);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);

    v.pop_back();
    EXPECT_EQ(v.size(), 1);
    EXPECT_EQ(v.capacity(), default_cap);
    EXPECT_EQ(v[0], 1);

    v.pop_back();
    EXPECT_EQ(v.size(), 0);
    EXPECT_EQ(v.capacity(), default_cap);
    EXPECT_TRUE(v.size() == 0);
}

TEST(VectorTest, RangeBasedLoop) {
    vector<int> vec = {1, 2, 3};
    int sum = 0;
    for (int elem: vec)
        sum += elem;
    EXPECT_EQ(sum, 6);
}

TEST(VectorTest, OperatorStream) {
    vector<int> vec = {1, 2, 3};
    std::stringstream oss;
    oss << vec;
    EXPECT_EQ(oss.str(), "[1, 2, 3]");
}

TEST(VectorTest, EraseTest) {
    mystd::vector<int> vec = {1, 2, 3, 4, 5};

    // Erase 3rd element (index 2)
    vec.erase(vec.begin() + 2);

    // Check size and contents
    ASSERT_EQ(vec.size(), 4);
    ASSERT_EQ(vec[0], 1);
    ASSERT_EQ(vec[1], 2);
    ASSERT_EQ(vec[2], 4);
    ASSERT_EQ(vec[3], 5);

    // Erase first and last elements
    vec.erase(vec.begin());
    vec.erase(vec.end() - 1);

    // Check size and contents
    ASSERT_EQ(vec.size(), 2);
    ASSERT_EQ(vec[0], 2);
    ASSERT_EQ(vec[1], 4);
}

#endif // FMI_OOP_USE_MYSTD_VECTOR
