#include <cmath>
#include <iostream>

static const size_t STR_MAX_SIZE = 128;

char decimal_to_umb14sym(int dec) {
	switch (dec) {
		case 0: return '0';
		case 1 ... 13: return 'a' + (dec - 1);
	}
	return '?';
}

// Convert ursamaior_base14 to ordinary earth_base14.
char umb14_to_b14(char sym) {
    switch (sym) {
	case '0':
	    return '0';
	case 'a' ... 'i':
	    return '1' + (sym - 'a');
	case 'j' ... 'm':
	    return 'a' + (sym - 'j');
    }
    // unreachable.
    return '?';
}

// Convert earth_base14 symbol to decimal number.
uint8_t b14_to_decimal(char sym) {
    switch (sym) {
	case '0' ... '9':
	    return sym - '0';
	case 'a' ... 'd':
	    return 10 + (sym - 'a');
    }
    // unreachable
    return 0xff;
}

// Convert a string of ursamaior_base14 symbols to a decimal num.
int umb14str_to_decimal(const char *str_begin, const char *str_end) {
    int decimal_num = 0;
    int digit_idx = str_end - str_begin - 1;
    for (const char *p = str_begin; p != str_end; ++p) {
	int dec = b14_to_decimal(umb14_to_b14(*p));
	decimal_num += dec * std::pow(14, digit_idx--);
    }
    return decimal_num;
}

static char umb14_num[STR_MAX_SIZE]; // stored in reverse order
static size_t umb14_num_sz = 0;
void decimal_to_umb14(int decimal_num) {
	int i;
	for (i = 0; decimal_num > 0; ++i, decimal_num /= 14)
		umb14_num[i] = decimal_to_umb14sym(decimal_num % 14);
	umb14_num_sz = i + 1;
}

void println_umb14(void) {
	for (int i = 1; i <= umb14_num_sz; ++i)
		std::cout << umb14_num[umb14_num_sz - i];
	std::cout << '\n';
}

bool is_date_in_weekend(int day, int month, int year) {
    month = (month + 9) % 12;
    year -= month / 10;
    int dyn_day = (365 * year + year / 4 - year / 100 + year / 400 +
		   (month * 306 + 5) / 10 + (day - 1)) %
		  7;
    return dyn_day != 3 && dyn_day != 4;
}

void handle_ursamaior_base14_date(const char *str) {
    const char *str_end = str + STR_MAX_SIZE;
    const char *str_begin = str;
    const char *p;
    for (p = str_begin; p != str_end && *p != '.'; ++p)
	;
    int day = umb14str_to_decimal(str_begin, p++);
    for (str_begin = p; p != str_end && *p != '.'; ++p)
	;
    int month = umb14str_to_decimal(str_begin, p++);
    for (str_begin = p; p != str_end && *p != '\0'; ++p)
	;
    int year = umb14str_to_decimal(str_begin, p);

    const bool in_weekend = is_date_in_weekend(day, month, year);
	std::cout << ((in_weekend) ? "No, " : "Yes, ") << day << '.' << month << '.' << year
		<< ((in_weekend) ? " isn't" : " is") << " a working day.\n";
}

int main() {
    char str_date[STR_MAX_SIZE + 1];
    std::cin.getline(str_date, STR_MAX_SIZE);
    int num;
    std::cin >> num;

    handle_ursamaior_base14_date(str_date);

	decimal_to_umb14(num);
	println_umb14();

    return 0;
}
