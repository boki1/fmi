#include <iostream>

#define MAX_STR_SIZE 2048
#define MAX_N 		 1024

static uint32_t fvn_hash(const char *p) {
    uint32_t r = 2166136261;
    for (; *p; p++) {
        r ^= *p;
        r *= 16777619;
    }
    return r;
}

size_t my_strlen(const char *str) {
	int sz = 0;
	while (*str++ != '\0') ++sz;
	return sz;
}

void my_strcpy(char *dest, const char *src) {
	while (*dest++ = *src++);
}

int get_hash_idx(uint32_t hashes[MAX_N], uint32_t target_hash, size_t limit) {
	for (int i = 0; i < limit; ++i)
		if (hashes[i] == target_hash)
			return i;
	return -1;
}

int main() {
	int n;
	std::cin >> n;

	char str[MAX_STR_SIZE + 1];
	// const char *str;

	int next = 0;
	uint32_t hashes[MAX_N] = {0};
	char *strings[MAX_N]{};
	int occurances[MAX_N] = {0};

	// const char *input[MAX_N] = {"world", "hello", "hello", "bye", "world", "hello", "hello"};

	for (int i = 0; i < n; ++i) {
		std::cin.getline(str, MAX_STR_SIZE);
		// str = input[i];
		uint32_t hash = fvn_hash(str);
		if (int idx = get_hash_idx(hashes, hash, i); idx != -1) {
			++occurances[idx];
			continue;
		}
		strings[next] = new char[my_strlen(str)];
		my_strcpy(strings[next], str);
		hashes[next] = hash;
		occurances[next] = 1;
		++next;
	}

	for (int i = 0; i < next; ++i)
		std::cout << strings[i] << " - " << occurances[i] << '\n';

	for (int i = 0; i < next; ++i)
		delete[] strings[i];

	return 0;
}
