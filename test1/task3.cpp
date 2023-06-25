#include <iostream>

static const int MAX_M = 256;
static const int MAX_N = 256;

int product_of_row(const int *row, std::size_t num_cols) {
	int prod = 1;
	for (std::size_t i = 0; i < num_cols; ++i)
		prod *= row[i];
	return prod;
}

// Ugly and inefficient but works :).
void sort(int *arr, std::size_t size) {
	for (int i = 0; i < size - 1; ++i) {
		for (int j = i + 1; j < size; ++j) {
			if (arr[i] > arr[j]) {
				int tmp = arr[i];
				arr[i] = arr[j];
				arr[j] = tmp;
			}
		}
	}
}

int find_row_with_product(int *orig_products, const std::size_t num_rows, const int desired_product) {
	for (int i = 0; i < num_rows; ++i) {
		if (orig_products[i] == desired_product)
			return i;
	}

	// should be unreachable given the current use of this function.
	// assert(!"unreachable");
	return -1;
}

void print_sorted_rows_by(int matrix[][MAX_M], std::size_t num_rows, std::size_t num_cols) {
	int orig_products[MAX_N] = {0};
	int sorted_products[MAX_N] = {0};
	for (int row = 0; row < num_rows; ++row) {
		sorted_products[row] = orig_products[row] = product_of_row(matrix[row], num_cols);
	}

	sort(sorted_products, num_rows);

	for (int row_product_idx = 0; row_product_idx < num_rows; ++row_product_idx) {
		const int row_product = sorted_products[row_product_idx];
		const int row_idx = find_row_with_product(orig_products, num_rows, row_product);
		if (row_idx < 0) {
			// requires <assert.h>
			// assert(!"unreachable");
		}

		// invalidate, as to handle duplicates
		orig_products[row_idx] = -1;

		const int *row = matrix[row_idx];
		for (int col = 0; col < num_cols; ++col) {
			std::cout << row[col] << ' ';
		}
		std::cout << '\n';
	}
}

int main() {
	int n; // # rows
	int m; // # columns
	std::cin >> n >> m;

	int inp_matrix[MAX_N][MAX_M] = {0};
	for (int i = 0, elem; i < n; ++i) {
		for (int j = 0; j < m; ++j) {
			std::cin >> elem;
			inp_matrix[i][j] = elem;
		}
	}

	print_sorted_rows_by(inp_matrix, n, m);

	return 0;
}
