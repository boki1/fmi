#include <iostream>
#include <cmath>

#include "mystd.h"

struct board {
    size_t dim;
    size_t num_cells;
    ssize_t *solution;

    explicit board(size_t n) : dim{n}, solution{new ssize_t[n * n]}, num_cells{n * n} {
        mystd::memset(solution, -1, num_cells * sizeof(ssize_t));
    }

    ~board() noexcept { delete[] solution; }

    struct pos {
        int x;
        int y;
    };

    struct move {
        int dx;
        int dy;
    };

    void dump_solution() const {
        for (int x = 0; x < dim; ++x) {
            for (int y = 0; y < dim; ++y)
                std::cout << solution[y * dim + x] << ' ';
            std::cout << '\n';
        }
        std::cout << '\n';
    }

    auto &at(const pos p) { return solution[p.y * dim + p.x]; }
    const auto &at(const pos p) const { return solution[p.y * dim + p.x]; }
    bool is_valid_pos(pos p) const { return !(p.x < 0 || p.y < 0) && !(p.x >= dim || p.y >= dim); }
    bool is_empty_pos(pos p) const { return at(p) == -1; }

    bool find_solution() {
        if (dim == 1)
            return true;
        if (dim <= 3)
            return false;

        // all moves which are possible with the horse
        static board::move moves[]{
                {-1, -2}, {+1, -2},
                {-1, +2}, {+1, +2},
                {-2, -1}, {-2, +1},
                {+2, -1}, {+2, +1},
        };

        auto search = [&](board::pos curr, std::size_t next, auto &&search) -> bool {
            at(curr) = next++;
            // std::string tabs(next, ' ');
            // std::cout << tabs << "setting (" << curr.x << ", " << curr.y << ") [#" << next - 1 << "]\n";
            if (next == num_cells)
                return true;

            for (auto move: moves) {
                const auto new_pos = board::pos{curr.x + move.dx, curr.y + move.dy};
                if (!is_valid_pos(new_pos) || !is_empty_pos(new_pos))
                    continue;
                // std::cout << tabs << "(try) ... ";
                if (search(new_pos, next, search))
                    return true;
                // std::cout << tabs << "failed going to (" << new_pos.x << ", " << new_pos.y << ") [#" << at(new_pos) << "]\n";
            }

            // std::cout << tabs << "resetting (" << curr.x << ", " << curr.y << ") [#" << at(curr) << "]\n";
            at(curr) = -1;
            return false;
        };

        board::pos initial{0, 0};
        return search(initial, 0, search);
    }

};

int main() {
    size_t n;
    std::cin >> n;
    auto brd = board{n};

    if (brd.find_solution()) {
        std::cout << "There is a solution!\n";
        brd.dump_solution();
    } else {
        std::cout << "No solution!\n";
    }

    return 0;
}
