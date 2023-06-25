#include <iostream>

static constexpr int MAX_ARRAY_SIZE = 1024;

const int *getSpecialElement(int *arr, int size) {
    if (size < 3)
        return nullptr;

	int *this_elem;
	for (this_elem = arr; this_elem < arr + (size - 1); ++this_elem) {
		const int *next_elem = this_elem + 1;
		if (*this_elem >= *next_elem)
			break;
	}

	// std::cout << "DEBUG: this_elem (special candidate) = " << *this_elem << '\n';
	const int *special_candidate = this_elem;
	for (; this_elem < arr + (size - 1); ++this_elem) {
		const int *next_elem = this_elem + 1;
		if (*this_elem < *next_elem) {
			// std::cout << "DEBUG: *this_elem (special candidate) is less than *next_elem : " << *this_elem << ", " << *next_elem << '\n';
			return nullptr;
		}
	}

	// std::cout << "DEBUG: special = " << *special_candidate << '\n';
	return special_candidate;
}

int main() {
    int inp_size;
    int inp[MAX_ARRAY_SIZE] = {0};

    std::cin >> inp_size;
    for (int i = 0; i < inp_size; ++i)
        std::cin >> inp[i];

	if (const int *special = getSpecialElement(inp, inp_size); special) {
		std::cout << "The array is special, the special element is " << *special << '\n';
	} else {
		std::cout << "The array is not special\n";
	}

    return 0;
}
