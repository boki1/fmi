#include <mystd/string.h>

#ifdef FMI_OOP_USE_MYSTD_STRING

#include <gtest/gtest.h>

#include <mystd/enable.h>
#include <mystd/string.h>

TEST(StringTest, DefaultConstructor) {
  mystd::string s;
  EXPECT_EQ("", s);
}

TEST(StringTest, ConstructorFromCharArray) {
  const char* str = "Hello";
  mystd::string s(str);
  EXPECT_EQ(str, s);
}

TEST(StringTest, CopyConstructor) {
  const char* str = "Hello";
  mystd::string s1(str);
  mystd::string s2(s1);
  EXPECT_EQ(s1, s2);
}

TEST(StringTest, AssignmentOperator) {
  const char* str1 = "Hello";
  const char* str2 = "World";
  mystd::string s1(str1);
  mystd::string s2(str2);
  s1 = s2;
  EXPECT_EQ(str2, s1);
}

TEST(StringTest, Size) {
  const char* str = "Hello";
  mystd::string s(str);
  EXPECT_EQ(5, s.size());
}

TEST(StringTest, AppendChar) {
  const char* str1 = "Hello";
  const char* str2 = "World";
  mystd::string s(str1);
  s.append(str2[0]);
  EXPECT_EQ(str2[0], s[s.size() - 1]);
}

TEST(StringTest, LexicographicCmp) {
  EXPECT_EQ(
      mystd::string{"Lord of the Rings"} < mystd::string{"Linux Weekly News"},
      std::string{"Lord of the Rings"} < std::string{"Linux Weekly News"});
}

#endif  // FMI_OOP_USE_MYSTD_STRING
