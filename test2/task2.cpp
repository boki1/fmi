#include <iostream>

#include "mystd.h"

struct encryption_algo {
    inline static const size_t password_length = 9;
    inline static const size_t alphabet_length = 'Z' - 'A' + 1; // we encrypt only english letters

    struct encryption_matrix {
        char dict_id_to_sym[alphabet_length]{};    // Eg: [0] = 'A', [1] = 'B', et c.
        int dict_sym_to_id[alphabet_length] = {-1};
        int dict_num_filled{0};

        int pswd_seq[password_length];
        int *text_seq;
        int text_num_filled{0};
        int text_num_filled_actual{0};

        mystd::string encrypted;

        encryption_matrix(int text_seq_size)
        : text_num_filled_actual{text_seq_size}
        {
            for (int i = 0; i < alphabet_length; ++i)
                dict_sym_to_id[i] = -1;

            if (const int rem = text_seq_size % 3; rem != 0)
                text_seq_size += (3 - rem);
            text_seq = new int[text_seq_size];
            for (int i = 0; i < text_seq_size; ++i)
                text_seq[i] = 0;
        }

        ~encryption_matrix() noexcept {
            delete[] text_seq;
        }
    };

    size_t encrypt_triplet(encryption_matrix &emx, int start_idx) {
        for (int i = 0, tmp = 0; i < 3; ++i, tmp = 0) {
            for (int k = 0; k < 3; ++k)
                tmp += emx.text_seq[start_idx + k] * emx.pswd_seq[i * 3 + k];
            tmp %= alphabet_length;
            const char res_sym = 'A' + rev_alpha_id(tmp);
            emx.encrypted.append(res_sym);
        }
    }

    static int rev_alpha_id(char sym) {
        if (sym >= 'A' && sym <= 'Z')
            return sym - 'A';
        if (sym >= 'a' && sym <= 'z')
            return sym - 'a';
        // unreachable!
    }

    void operator()(const mystd::string &text, const mystd::string &password) {
        encryption_matrix emx{text.size};

        // assert(password.size == password_length);
        for (int i = 0; i < password_length; ++i) {
            const auto rev_id = rev_alpha_id(password.ptr[i]);
            if (emx.dict_sym_to_id[rev_id] != -1)
                continue;
            emx.dict_id_to_sym[i] = password.ptr[i];
            emx.dict_sym_to_id[rev_id] = i;
        }
        emx.dict_num_filled = password_length;

        for (int i = 0; i < password_length; ++i)
            emx.pswd_seq[i] = emx.dict_sym_to_id[i];

        for (int i = 0; i < text.size; ++i) {
            const auto rev_id = rev_alpha_id(text.ptr[i]);
            if (emx.dict_sym_to_id[rev_id] == -1) {
                emx.dict_id_to_sym[emx.dict_num_filled] = text.ptr[i];
                emx.dict_sym_to_id[rev_id] = emx.dict_num_filled;
                ++emx.dict_num_filled;
            }
            emx.text_seq[emx.text_num_filled++] = emx.dict_sym_to_id[rev_id];
        }

        const int rem = emx.text_num_filled % 3;
        const int to_add = 3 - rem;
        emx.text_num_filled += to_add;

//#define DEBUG
#ifdef DEBUG
        std::cout << "emx.dict_id_to_sym:\n";
        for (int i = 0; i < emx.dict_num_filled; ++i)
            std::cout << i << ' ' << emx.dict_id_to_sym[i] << '\n';
        std::cout << "emx.dict_sym_to_id:\n";
        for (int i = 0; i < emx.dict_num_filled; ++i)
            std::cout << i << ' ' << emx.dict_sym_to_id[i] << '\n';
        std::cout << "emx.pswd_seq:\n";
        for (int i = 0; i < password_length; ++i)
            std::cout << emx.pswd_seq[i] << ' ';
        std::cout << '\n';
        std::cout << "emx.text_seq:\n";
        for (int i = 0; i < emx.text_num_filled; ++i)
            std::cout << emx.text_seq[i] << ' ';
        std::cout << '\n';
#endif

        for (int i = 0; i < emx.text_num_filled; i+=3) {
            encrypt_triplet(emx, i);
        }

        if (to_add != 3)
        for (int i = 0; i < to_add; ++i)
            emx.encrypted.pop_last();
        std::cout << "Encrypted: " << emx.encrypted << '\n';

    }
};

int main() {
    mystd::string text;
    mystd::string password;

    std::cin >> text;
    std::cin >> password;Q

    encryption_algo a;
    a(text, password);

    return 0;
}