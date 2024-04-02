#include <assert.h>

#include "heap.h"

#define MUST(a) \
    { int b = (a); assert(b); }

#define DIM(a) \
	sizeof(a) / sizeof(*a)

static bool is_sorted(int* arr, size_t size)
{
	for (size_t i = 0; i < size - 1; ++i)
		if (arr[i] > arr[i + 1])
			return false;
	return true;
}

int main(int argc, char* argv[])
{
	int arr[] =
		{ -32, -35, 40, -53, 13, -58, -75, -41, 44, -83, 21, 25, -43, 47, -1, -9, -16, -20, -61, -50, -63, -76, 58, -49, 19,
			-56, 11, 39, 85, 12, 64, 62, -42, 94, 18, -94, -24, -93, 95, 72, -59, 33, 63, 35, 14, 70, -4, -12, -5, -2, 7, -3,
			-62, -88, 88, -100, 92, 65, -81, 16, 100, 84, 86, 36, 32, -68, -74, -90, -51, -25, -22, -15, 0, -47, -37, 30, 29,
			3, 15, 8, -96, 17, 76, -69, -57, 28, -86, 5, 98, 27, 56, -60, -82, 67, 78, -46, 55, -10, 79, -84, 26, -99, 54,
			-67, 74, -44, -80, -98, 77, 10, -55, 6, 24, 45, -21, 75, 43, -36, 80, -31, 71, -38, -65, -11, -54, -87, -77, 2,
			-92, 49, 68, 9, -78, -29, -91, -70, -95, 51, -45, -33, 73, -64, 66, -6, 34, 91, -27, 20, 4, -73, 50, 81, -48, -14,
			59, -19, 90, -30, 22, 57, 31, -66, -34, -18, 52, -17, 99, 93, 37, -7, -26, -13, 53, -97, -52, -89, 60, 83, -28,
			23, -85, 82, 69, 97, -79, 46, -72, -71, -8, 1, 87, 96, 42, 48, 38, -39, -23, 89, -40, 61, 41
		};

	struct heap *h = heap_init(arr, DIM(arr));
	int res=-0xAA;
	MUST(heap_extract_max(h, &res));
	MUST(res == 100);

	heap_insert(h, 0xffff);
	MUST(heap_extract_max(h, &res));
	MUST(res == 0xffff);

	MUST(heap_extract_max(h, &res));
	MUST(res == 99);

	MUST(heap_max(h, &res));
	MUST(res == 98);

	heap_sort(arr, DIM(arr));
	assert(is_sorted(arr, DIM(arr)));

	heap_destroy(h);

	return 0;
}
