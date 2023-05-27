#include <chrono>
#include <ostream>
using namespace std::chrono_literals;

#include <gtest/gtest.h>

#include <libhw2/Borrow.h>
#include <libhw2/Library.h>
#include <libhw2/User.h>

#include <libhw2/texts/Book.h>
#include <libhw2/texts/Comics.h>
#include <libhw2/texts/Genre.h>
#include <libhw2/texts/Periodical.h>
#include <libhw2/texts/Text.h>

#include <libhw2/builders/BookBuilder.h>
#include <libhw2/builders/ComicsBuilder.h>
#include <libhw2/builders/PeriodicalBuilder.h>
#include <libhw2/builders/TextBuilder.h>

using namespace libhw2;

class LibraryTest : public ::testing::Test {
 public:
  User froddo{"Froddo Baggins"};
  User sam{"Samwise Gamgee"};
  User bilbo{"Bilbo Baggins"};
  Library shire_library{"The Local Shire Library"};
  Library gondor_library{"The Big Ancient Gondor Library"};

  const Text lord_of_the_rings =
      *LibraryTest::make_lord_of_the_rings().value().get();
  const Text lwn_shire_edition =
      *LibraryTest::make_lwn_shire_edition().value().get();

  static TextBuilder::ResultType make_lord_of_the_rings() {
    BookBuilder book_builder;
    Genre lord_of_the_rings_genre{Genre::Characteristic::Fantasy,
                                  Genre::Type::Prose,
                                  Genre::TargetAudience::Above12};
    book_builder.with_details(
        "Lord of the Rings",
        "This is a legendary high-fantasy epic by J. R. R. Tolkien.", 1954y);
    book_builder.with_authored_details(
        "J. R. R. Tolkien", "George Allen & Unwin", lord_of_the_rings_genre);

    return book_builder.build();
  }

  static TextBuilder::ResultType make_lwn_shire_edition() {
    PeriodicalBuilder periodical_builder;

    periodical_builder.with_details(
        "Linux Weekly News",
        "Reader-supported news site dedicated to producing the best coverage "
        "from within the Linux and free software development communities",
        2023y);

    periodical_builder.with_periodical_details(PeriodicalTextTag::Period::Daily,
                                               42);

    return periodical_builder.build();
  }
};

TEST_F(LibraryTest, Construction) {
  EXPECT_NE(shire_library.name(), gondor_library.name());

  Library cpy{shire_library};
  EXPECT_NE(cpy.name(), gondor_library.name());

  EXPECT_TRUE(shire_library.registered_users().empty());
  EXPECT_TRUE(shire_library.texts().empty());
}

TEST_F(LibraryTest, ManageUsers) {
  EXPECT_TRUE(shire_library.registered_users().empty());
  const bool added_froddo = shire_library.add_user(froddo);
  EXPECT_TRUE(added_froddo);
  EXPECT_EQ(shire_library.registered_users().size(), 1);
  const bool removed_froddo = shire_library.remove_user(froddo.id());
  EXPECT_TRUE(removed_froddo);
  const bool removed_froddo_again = shire_library.remove_user(froddo.id());
  EXPECT_FALSE(removed_froddo_again);
  EXPECT_EQ(shire_library.registered_users().size(), 0);
}

TEST_F(LibraryTest, BuildTexts) {
  const auto& maybe_lord_of_the_rings = LibraryTest::make_lord_of_the_rings();
  EXPECT_TRUE(maybe_lord_of_the_rings.has_value());
  const Book& lord_of_the_rings =
      *dynamic_cast<const Book*>(maybe_lord_of_the_rings.value().get());

  EXPECT_EQ(lord_of_the_rings.title(),
            "Lord of the Rings");
  EXPECT_EQ(lord_of_the_rings.author(), "J. R. R. Tolkien");

  const auto maybe_lwn = LibraryTest::make_lwn_shire_edition();
  EXPECT_TRUE(maybe_lwn.has_value());
  const Periodical& lwn =
      *dynamic_cast<const Periodical*>(maybe_lwn.value().get());
  EXPECT_EQ(lwn.number(), 42);
}

TEST_F(LibraryTest, ManageTexts) {
  EXPECT_TRUE(shire_library.texts().empty());

  {
    const auto available_copies_of_lotr =
        shire_library.add_text(lord_of_the_rings);
    EXPECT_EQ(available_copies_of_lotr, 1);
    EXPECT_EQ(shire_library.texts().size(), 1);
  }
  {
    const auto available_copies_of_lotr =
        shire_library.add_text(lord_of_the_rings);
    EXPECT_EQ(available_copies_of_lotr, 2);
    EXPECT_EQ(shire_library.texts().size(), 1);
  }

  {
    const auto available_copies_of_lwn =
        shire_library.add_text(lwn_shire_edition);
    EXPECT_EQ(available_copies_of_lwn, 1);
    EXPECT_EQ(shire_library.texts().size(), 2);
  }

  {
    const auto available_copies_of_lotr =
        shire_library.remove_text(lord_of_the_rings.id());
    EXPECT_EQ(available_copies_of_lotr, 1);
    EXPECT_EQ(shire_library.texts().size(), 2);
  }

  {
    const auto available_copies_of_lotr =
        shire_library.remove_text(lord_of_the_rings.id());
    EXPECT_EQ(available_copies_of_lotr, 0);
    EXPECT_EQ(shire_library.texts().size(), 1);
  }

  {
    const auto available_copies_of_lotr =
        shire_library.add_text(lord_of_the_rings);
    EXPECT_EQ(available_copies_of_lotr, 1);
    EXPECT_EQ(shire_library.texts().size(), 2);
  }

  {
    const auto available_copies_of_lotr =
        shire_library.remove_text(lord_of_the_rings.id());
    EXPECT_EQ(available_copies_of_lotr, 0);
    EXPECT_EQ(shire_library.texts().size(), 1);
  }

  {
    const auto available_copies_of_lwn =
        shire_library.remove_text(lwn_shire_edition.id());
    EXPECT_EQ(available_copies_of_lwn, 0);
    EXPECT_EQ(shire_library.texts().size(), 0);
  }
}

TEST_F(LibraryTest, ManageUsersAndTexts) {
  const Text lord_of_the_rings =
      *LibraryTest::make_lord_of_the_rings().value().get();
  const Text lwn_shire_edition =
      *LibraryTest::make_lwn_shire_edition().value().get();
  for (int i = 0; i < 10; ++i) {
    (void)shire_library.add_text(lord_of_the_rings);
    (void)shire_library.add_text(lwn_shire_edition);
  }
  (void)shire_library.add_user(froddo);
  (void)shire_library.add_user(sam);
  EXPECT_EQ(froddo.active_readings(), 0);
  EXPECT_EQ(sam.active_readings(), 0);
  EXPECT_EQ(shire_library.texts().size(), 2);

  for (int i = 0; i < 4; ++i) {
    const auto lend_to_froddo =
        shire_library.lend_text(froddo, lord_of_the_rings.id());
    EXPECT_TRUE(lend_to_froddo);
    const auto lend_to_sam =
        shire_library.lend_text(sam, lord_of_the_rings.id());
    EXPECT_TRUE(lend_to_sam);
  }

  const bool remove1 = shire_library.remove_text(lord_of_the_rings.id());
  EXPECT_TRUE(remove1);  // 2 copies of LOTR are available.

  const bool remove2 = shire_library.remove_text(lwn_shire_edition.id());
  EXPECT_TRUE(remove2);  // LWN has 10 available copies.

  EXPECT_EQ(shire_library.check_text(lwn_shire_edition.id()), 9);
  EXPECT_EQ(shire_library.check_text(lord_of_the_rings.id()), 1);

  const auto maybe_lend_to_froddo =
      shire_library.lend_text(froddo, lord_of_the_rings.id());
  EXPECT_TRUE(maybe_lend_to_froddo);
  [[maybe_unused]] const auto lend_to_froddo_lord_of_the_rings_borrow_id =
      maybe_lend_to_froddo.value();

  const auto maybe_lend_to_sam =
      shire_library.lend_text(sam, lord_of_the_rings.id());
  EXPECT_FALSE(maybe_lend_to_sam);
  EXPECT_EQ(shire_library.check_text(lord_of_the_rings.id()), 0);

  shire_library.return_text(lend_to_froddo_lord_of_the_rings_borrow_id);
  EXPECT_EQ(shire_library.check_text(lord_of_the_rings.id()), 1);

  const auto maybe_lend_to_sam2 =
      shire_library.lend_text(sam, lord_of_the_rings.id());
  EXPECT_TRUE(maybe_lend_to_sam2);
  EXPECT_EQ(shire_library.check_text(lord_of_the_rings.id()), 0);

  EXPECT_EQ(froddo.active_readings(), 5);
  EXPECT_EQ(sam.active_readings(), 5);
}

TEST_F(LibraryTest, SortUsers) {
  for (int i = 0; i < 6; ++i) {
    (void)shire_library.add_text(lord_of_the_rings);
    (void)shire_library.add_text(lwn_shire_edition);
  }
  (void)shire_library.add_user(froddo);
  (void)shire_library.add_user(sam);
  (void)shire_library.add_user(bilbo);

  for (int i = 0; i < 3; ++i) {
    const auto maybe_borrow_id =
        shire_library.lend_text(froddo, lord_of_the_rings.id());
    shire_library.return_text(maybe_borrow_id.value());
  }

  for (int i = 0; i < 2; ++i) {
    const auto maybe_borrow_id =
        shire_library.lend_text(sam, lord_of_the_rings.id());
    shire_library.return_text(maybe_borrow_id.value());
  }

  (void)shire_library.lend_text(froddo, lord_of_the_rings.id());
  (void)shire_library.lend_text(froddo, lord_of_the_rings.id());
  (void)shire_library.lend_text(sam, lord_of_the_rings.id());
  (void)shire_library.lend_text(sam, lord_of_the_rings.id());

  const auto sorted_users = shire_library.sort_users();

  // EXPECTED: FRODDO > SAM > BILBO (in reverse of course).
  EXPECT_EQ(sorted_users.at(2).id(), froddo.id());
  EXPECT_EQ(sorted_users.at(1).id(), sam.id());
  EXPECT_EQ(sorted_users.at(0).id(), bilbo.id());
}

TEST_F(LibraryTest, UsersInDebt) {
  for (int i = 0; i < 5; ++i) {
    (void)shire_library.add_text(lord_of_the_rings);
    (void)shire_library.add_text(lwn_shire_edition);
  }
  (void)shire_library.add_user(froddo);
  (void)shire_library.add_user(sam);
  (void)shire_library.add_user(bilbo);

  auto sstr = shire_library.print_texts();
  EXPECT_EQ(
      sstr.str(),
      "Text { .title = \"Linux Weekly News\", .abstract = \"Reader-supported "
      "news site dedicated to producing the best coverage from within the "
      "Linux and free software development communities\", .id = 17 }Text { "
      ".title = \"Lord of the Rings\", .abstract = \"This is a legendary "
      "high-fantasy epic by J. R. R. Tolkien.\", .id = 16 }");
}
