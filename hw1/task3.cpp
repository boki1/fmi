#include <iostream>

#define MAX_N 9

enum class game_status { playing, x_won, o_won, draw };

bool sym_has_row(int table[MAX_N][MAX_N], int n, char sym) {
	for (int i = 0; i < n; ++i) { // rows
		bool has_full_row = true;
		for (int j = 0; j < n - 1; ++j) // cols
			if (table[i][j] != sym) {
				has_full_row = false;
				break;
			}
		if (has_full_row)
			return true;
	}

	return true;
}

bool sym_has_col(int table[MAX_N][MAX_N], int n, char sym) {
	for (int i = 0; i < n; ++i) { // rows
		bool has_full_col = true;
		for (int j = 0; j < n; ++j) // cols
			if (table[j][i] != sym) {
				has_full_col = false;
				break;
			}
		if (has_full_col)
			return true;
	}

	return true;
}

bool sym_has_diag(int table[MAX_N][MAX_N], int n, char sym) {
	bool has_main_diag = true;
	for (int i = 0; i < n; ++i)
		if (table[i][i] != sym) {
			has_main_diag = false;
			break;
		}

	if (has_main_diag)
		return true;

	for (int i = 0; i < n; ++i)
		if (table[i][n - 1] != sym)
			return false;

	return true;
}

int empty_space(int table[MAX_N][MAX_N], int n) {
	int empty = 0;
	for (int i = 0; i < n; ++i)
		for (int j = 0; j < n; ++j)
			if (table[i][j] == 0)
				++empty;
	return empty;
}

game_status is_game_over(int table[MAX_N][MAX_N], int n) {
	if (sym_has_col(table, n, 'X') || sym_has_row(table, n, 'X') || sym_has_diag(table, n, 'X'))
		return game_status::x_won;

	if (sym_has_col(table, n, 'O') || sym_has_row(table, n, 'O') || sym_has_diag(table, n, 'O'))
		return game_status::o_won;

	if (empty_space(table, n) == 0)
		return game_status::draw;

	return game_status::playing;
}

bool are_valid_coords(int table[MAX_N][MAX_N], int x, int y, int n) {
	if (x < 0 || x >= n) return false;
	if (y < 0 || y >= n) return false;
	if (table[x][y] != 0) return false;
	return true;
}

int main() {
	int n;
	std::cin >> n;
	int table[MAX_N][MAX_N]{};

	char sym = 'X';
	int x, y;
	game_status status;
	while ((status = is_game_over(table, n)) == game_status::playing) {
		sym = (sym == 'X' ? 'Y' : 'X');
		std::cout << "It is " << sym << "'s turn.\n'";
		std::cin >> x >> y;
		if (are_valid_coords(table, x, y, n))
			table[x][y] = sym;
	}

	switch (status) {
		break; case game_status::draw: std::cout << "Draw.\n";
		break; case game_status::x_won: std::cout << "X won.\n";
		break; case game_status::o_won: std::cout << "O won.\n";
	}

	return 0;
}
