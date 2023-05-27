#ifndef FMI_OOP_HW2_USER_H
#define FMI_OOP_HW2_USER_H

#include <mystd/vector.h>
#include <mystd/map.h>

#include <libhw2/Library.h>
#include <libhw2/texts/Text.h>

namespace libhw2 {

class User {
public:
	enum class ReadingStatus : std::uint8_t {
		Completed, InProgress, Returned
	};

	struct ReadingHistory {
		mystd::vector<ReadingStatus> m_history;
	};

private:
	mystd::map<Library::Id, ReadingHistory> m_readings;

public: // Unintuitive syntax demanded by the task description
	bool operator==(const User &);
	bool operator!=(const User &);
	int operator<(const User &);
	int operator<=(const User &);
	int operator>(const User &);
	int operator>=(const User &);
	int operator[](Library::Id);
	int operator+(Text &);
	int operator+=(Text &);
	int operator-(Text &);
	int operator-=(Text &);

public: // Actual operations of User
	auto operator<=>(const User &);
	ReadingStatus status_of_text(Library::Id);
	void borrow_text(Library::Id);
	void return_text(Library::Id);
};

}

#endif // FMI_OOP_HW2_USER_H
