#include <iostream>
#include <cmath>

/*
 * "Средна характеристика на бактерия се нарича средно аритметично от трите й характеристики.
 * Ако една бактерия е заразена, то всичките й характеристики стават 0. Една заразена бактерия заразява друга, ако са
 * една до друга и разликата на средните им характеристики не е била по-голяма от 1."
 *
 * В условието има грешка, която води до противеоречие и невъзможност да се реши задачата по начина, по който е описана.
 * За да бъде заразена бактерия, то има две изисквания:
 *  1. Да е в съседство до вече заразена и
 *  2. Да има разлика на средните хар. <= 1
 * За целта обаче трябва да има една вече заразена бактерия, за да се започне цялата "инфекция". В това решение вместо
 * изискванията да са 1. и 2., аз съм реализирал да бъде 1. или 2.
 */

using bactery = uint8_t[3];
using bactery_colony = bactery **;
using imm_bactery_colony = const bactery *const *;

int bactery_avg(const bactery &self) {
    int sum = 0;
    for (int i = 0; i < 3; ++i) sum += self[i];
    return static_cast<int>(sum / 3);
}

void bactery_infect(bactery &self) {
    self[0] = self[1] = self[2] = 0;
}

bool bactery_is_infected(const bactery &self) {
    return (self[0] | self[1] | self[2]) == 0;
}

bool bactery_infects_neighbour(const bactery &self, const bactery &neighbour) {
    static constexpr int infection_threshold = 1;
    const auto self_avg = bactery_avg(self);
    const auto neighbour_avg = bactery_avg(neighbour);
    const auto infection_by_avg = std::abs(self_avg - neighbour_avg) <= infection_threshold;
    const auto infection_by_already_infected_bactery = bactery_is_infected(self);
    return infection_by_avg || infection_by_already_infected_bactery;
}

bactery_colony make_bactery_colony(std::size_t rows, std::size_t cols) {
    auto **colony = new bactery *[rows];
    for (int i = 0; i < rows; ++i)
        colony[i] = new bactery[cols];
    return colony;
}

bactery_colony copy_bactery_colony(std::size_t rows, std::size_t cols, imm_bactery_colony &colony) {
    auto new_colony = make_bactery_colony(rows, cols);
    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < cols; ++j)
            for (int k = 0; k < 3; ++k)
                new_colony[i][j][k] = colony[i][j][k];
    return new_colony;
}

void destroy_bactery_colony(bactery_colony colony, std::size_t rows, std::size_t cols) {
    for (int i = 0; i < rows; ++i)
        delete[] colony[i];
    delete[] colony;
}

std::size_t
bactery_neighbours_in_colony(bactery_colony colony, std::size_t rows, std::size_t cols, int x, int y,
                             bactery *neighbours[9]) {
    std::size_t neighbour_count = 0;
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (i == 0 && j == 0) continue; // skip current element
            if (x + i >= 0 && x + i < rows && y + j >= 0 && y + j < cols) {
                neighbours[neighbour_count++] = &colony[x + i][y + j];
            }
        }
    }

    return neighbour_count;
}

bactery_colony
infect(imm_bactery_colony colony, std::size_t rows, std::size_t cols, std::size_t target_row,
       std::size_t target_col) {
    bool infected_target = false;
    auto infected_colony = copy_bactery_colony(rows, cols, colony);

    while (true) {
        if (bactery_is_infected(infected_colony[target_row][target_col]))
            break;
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                auto &self = (uint8_t (&)[3]) colony[i][j];

                bactery *neighbours[9];
                std::size_t neighbour_cnt = bactery_neighbours_in_colony(infected_colony, rows, cols, i, j, neighbours);
                for (int n = 0; n < neighbour_cnt; ++n) {
                    bactery &neighbour = *neighbours[n];
                    if (bactery_infects_neighbour(self, neighbour))
                        bactery_infect(neighbour);
                    if (bactery_infects_neighbour(neighbour, self))
                        bactery_avg(self);
                }
            }
        }
    }

    return infected_colony;
}

// Very ugly workaround about the problem with implicit casting of
// uint8_t to char and reading chars from istream.
std::istream &operator>>(std::istream &is, uint8_t &i8) {
    int big;
    is >> big;
    i8 = static_cast<uint8_t>(big);
    return is;
}

int main() {
    int rows, cols;
    std::cin >> cols >> rows;
    auto **colony = make_bactery_colony(rows, cols);

    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < cols; ++j)
            for (int k = 0; k < 3; ++k)
                std::cin >> colony[i][j][k];

    int target_x, target_y;
    std::cin >> target_x >> target_y;

    bactery_colony infected_colony = infect(colony, rows, cols, target_x, target_y);

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j)
            std::cout << bactery_avg(infected_colony[i][j]) << ' ';
        std::cout << '\n';
    }

    destroy_bactery_colony(colony, rows, cols);
    destroy_bactery_colony(infected_colony, rows, cols);

    return 0;
}