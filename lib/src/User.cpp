#include <libhw2/User.h>

namespace libhw2 {

bool User::operator==(const User&) { }

bool User::operator!=(const User&) { }

int User::operator<(const User&) { }

int User::operator<=(const User&) { }

int User::operator>(const User&) { }

int User::operator>=(const User&) { }

int User::operator[](Library::Id) { }

int User::operator+(Text&) { }

int User::operator+=(Text&) { }

int User::operator-(Text&) { }

int User::operator-=(Text&) { }

auto User::operator<=>(const User&) { }

User::ReadingStatus User::status_of_text(Library::Id) { }

void User::borrow_text(Library::Id) { }

void User::return_text(Library::Id) { }

}
