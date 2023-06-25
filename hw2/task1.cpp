#include <cmath>
#include <iostream>

#include "mystd.h"

struct fib_gen {
	// start off at 2
    inline static int prev{2};
    inline static int pprev{1};

    static int next() {
		int tmp = prev + pprev;
		pprev = prev;
		return (prev = tmp);
	}

    static void reset() {
		prev = 2;
		pprev = 1;
	}
};

struct dynarray {
    std::size_t size{0};
    std::size_t capacity{2};
    double *pdata{new double[2]};

	void resize() {
		const std::size_t new_capacity = fib_gen::next();
		mystd::assert(capacity < new_capacity && size < new_capacity, "Bad new capacity!");

		double *new_pdata = new double[new_capacity];
		mystd::memcpy(new_pdata, pdata, size * sizeof(*pdata));
        delete[] pdata;

		pdata = new_pdata;
		capacity = new_capacity;
	}

	void push(double new_element) {
		// we have space left just for this new element, so resize in advance.
		if (size == capacity)
			resize();
		pdata[size] = new_element;
		++size;
	}

	~dynarray() noexcept {
		delete[] pdata;
	}
};

namespace mytest {
	void test_fibgen() {
		for (int i = 0; i < 10; ++i)
			std::cout << fib_gen::next() << ' ';
		std::cout << '\n';

		// don't corrupt other tests...
		fib_gen::reset();
	}

	void test_dynarray() {
		auto da = dynarray{};
		for (int i = 0; i < 100; ++i)
			da.push(static_cast<double>(i));
		std::cout << "Capacity: " << da.capacity << '\n';
		mystd::assert(da.capacity == 144, "TEST: Bad capacity");
		std::cout << "Size: " << da.size << '\n';
		mystd::assert(da.size == 100, "TEST: Bad size");

		std::cout << "Elements: ";
		for (int i = 0; i < da.size; ++i)
			std::cout << da.pdata[i] << ' ';
		std::cout << '\n';
	}

	void test() {
		test_fibgen();
		test_dynarray();
	}
}


int main() {
#ifdef DEBUG
		mytest::test();
#else
    auto da = dynarray{};
    while (true) {
        double inp;
        std::cin >> inp;
        if (inp == 0)
            break;
        da.push(inp);
    }

    std::cout << "Size: " << da.size << '\n';
    std::cout << "Capacity: " << da.capacity << '\n';

    std::cout.precision(2); // so is in the examples given
    std::cout << std::fixed;
    std::cout << "Data: ";
    if (da.size > 0) {
        for (int i = 0; i < da.size; ++i)
            std::cout << da.pdata[i] << ' ';
    } else {
        std::cout << "Array is empty!";
    }

    std::cout << '\n';
#endif
	return 0;
}
