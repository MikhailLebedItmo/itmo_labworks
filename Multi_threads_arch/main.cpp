#include "hit.h"

#include <cstdint>
#include <omp.h>
#include <vector>
#include <atomic>
#include <iostream>
#include <algorithm>
#include <numeric>
#include <fstream>


float xor_shift_64_rand(uint64_t& seed) {
    seed ^= seed << 13;
    seed ^= seed >> 7;
    seed ^= seed << 17;
    return (uint32_t)seed / (float)0xFFFFFFFFu;
}

struct Cords {
    float x;
    float y;
    float z;
};


class CmdArgsParser {
public:
    CmdArgsParser(int argc, char** argv): argc(argc), argv(argv), pos(1) {

    }

    void Parse() {
        while (pos < argc) {
            ParseOneArg();
        }
    }

private:
    void ParseOneArg() {
        if (argv[pos][0 + 2] == 'i') {
            input = std::string(argv[++pos]);
            ++pos;
            return;
        }
        if (argv[pos][0 + 2] == 'o' && argv[pos][1 + 2] == 'u') {
            output = std::string(argv[++pos]);
            ++pos;
            return;
        }
        if (argv[pos][0 + 2] == 'o') {
            ++pos;
            if (argv[pos][0] == 'd') {
                num_threads = default_thread_num;
                ++pos;
                return;
            }
            num_threads = strtol(argv[pos], nullptr, 10);
            ++pos;
            return;
        }
        ++pos;
        num_threads = 0;
    }

private:
    static int default_thread_num;
    int argc;
    char** argv;
    int pos;
public:
    std::string input;
    std::string output;
    int num_threads;
};

int CmdArgsParser::default_thread_num = omp_get_max_threads();

int main(int argc, char** argv) {
    CmdArgsParser arg_parser(argc, argv);
    arg_parser.Parse();
    std::ifstream fin(arg_parser.input);
    uint64_t n;
    fin >> n;

    const int thread_count = arg_parser.num_threads;

    auto axis_range = get_axis_range();
    auto x_min = axis_range[0];
    auto x_diff = axis_range[1] - x_min;
    auto y_min = axis_range[2];
    auto y_diff = axis_range[3] - y_min;
    auto z_min = axis_range[4];
    auto z_diff = axis_range[5] - z_min;
    uint64_t hit_cnt = 0;
    double begin = omp_get_wtime();
    if (thread_count > 0) {

#pragma omp parallel num_threads(thread_count)
        {
            uint64_t seed = omp_get_thread_num() + 0xb7f2C5;
            uint64_t hit_counter = 0;
#pragma omp for schedule(static)
            for (uint64_t i = 0; i < n; ++i) {
                Cords coords{xor_shift_64_rand(seed) * x_diff + x_min
                        , xor_shift_64_rand(seed) * y_diff + y_min
                        , xor_shift_64_rand(seed) * z_diff + z_min};
                hit_counter += hit_test(coords.x, coords.y, coords.z);
            }
#pragma omp atomic
            hit_cnt += hit_counter;
        }
    } else {
        uint64_t seed = 0xb7f2C5;
        for (uint64_t i = 0; i < n; ++i) {
            Cords coords{xor_shift_64_rand(seed) * x_diff + x_min
                    , xor_shift_64_rand(seed) * y_diff + y_min
                    , xor_shift_64_rand(seed) * z_diff + z_min};
            hit_cnt += hit_test(coords.x, coords.y, coords.z);
        }
    }
    printf("Time (%i thread(s)): %g ms\n", thread_count, 1000 * (omp_get_wtime() - begin));
//    printf("%g", 1000 * (omp_get_wtime() - begin));
    std::ofstream fout(arg_parser.output);
    fout << (float)hit_cnt / n * (x_diff * y_diff * z_diff) << '\n';
    return 0;
}