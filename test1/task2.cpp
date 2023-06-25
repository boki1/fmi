#include <iostream>

static constexpr int MAX_ARRAY_SIZE = 1024;

bool nums_are_prime(int a, int b) {
	return (a % b == 0) || (b % a == 0);
}

int main() {
	int inp;
	int n;
	std::cin >> n;

	// Rationale for +1: 0 is not used, but we do not want to mess up with the indexing,
	// so just skip the first element of the array.
	bool prime[MAX_ARRAY_SIZE + 1] = {true};
	while (1) {
		std::cin >> inp;
		if (inp == 0)
			break;

		for (int i = 1; i <= n; ++i) {
			const bool nrp = nums_are_prime(inp, i);
			// std::cout << std::boolalpha << "DEBUG: nums_are_prime(" << inp << ", " << i << ") = " << nrp << '\n';
			prime[i] |= nrp;
		}
	}

	for (int i = 1; i <= n; ++i)
		if (!prime[i])
			std::cout << i << ' ';
	std::cout << '\n';

    return 0;
}
