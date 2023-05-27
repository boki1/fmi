#include <gtest/gtest.h>

#include <libhw2/User.h>

using namespace libhw2;

class UserTest : public ::testing::Test {
 public:
  User froddo{"Froddo Baggins"};
  User sam{"Samwise Gamgee"};
};

TEST_F(UserTest, Construction) {
  EXPECT_NE(froddo.id(), sam.id());

  EXPECT_EQ(froddo.active_readings(), 0);
  EXPECT_TRUE(froddo.history().empty());

  froddo.set_name("Changed");
  EXPECT_EQ(froddo.name(), "Changed");
  froddo.set_name("Froddo Baggins");
  EXPECT_EQ(froddo.name(), "Froddo Baggins");
}

TEST_F(UserTest, Marking) {
	const auto text_id = TextId{};
	const auto borrow_obj = Borrow{text_id, froddo.id()};
  EXPECT_EQ(froddo.check_text(text_id), User::TextStatus::Unknown);

  froddo.mark_borrowed(borrow_obj);
  EXPECT_EQ(froddo.check_text(text_id), User::TextStatus::CurrentlyReading);
  froddo.mark_returned(borrow_obj);
  EXPECT_EQ(froddo.check_text(text_id), User::TextStatus::AlreadyRead);

  froddo.mark_borrowed(borrow_obj);
  froddo.mark_borrowed(borrow_obj);
  EXPECT_EQ(froddo.check_text(text_id), User::TextStatus::CurrentlyReading);
  froddo.mark_returned(borrow_obj);
  EXPECT_EQ(froddo.check_text(text_id), User::TextStatus::CurrentlyReading);
  froddo.mark_returned(borrow_obj);
  EXPECT_EQ(froddo.check_text(text_id), User::TextStatus::AlreadyRead);

  // Should go with some assert fatalities.
  // EXPECT_ANY_THROW(froddo.mark_returned(Borrow{42}));
}
