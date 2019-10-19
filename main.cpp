#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <chrono>

static unsigned int g_seed;

inline void fast_srand(int seed) {
    g_seed = seed;
}

inline int fastrand() {
    g_seed = (214013*g_seed+2531011);
    return (g_seed>>16)&0x7FFF;
}

void shuffle_deck(unsigned int arr[], const size_t size) {
    for (size_t i = 0; i < size; i++) {
        unsigned int tmp = arr[i];
        unsigned int rand = fastrand() % size;
        arr[i] = arr[rand];
        arr[rand] = tmp;
    }
}

int main() {
    using namespace std::chrono;
    const unsigned int rounds = 10000000U;
    const size_t deckSize = 52;
    unsigned int lost = 0;
    unsigned int deck[deckSize] = {
        1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3
    };
    steady_clock::time_point start = steady_clock::now();

    fast_srand(static_cast<int>(std::time(NULL)));
    for (unsigned int played = 0; played < rounds; played++) {
        shuffle_deck(deck, deckSize);
        unsigned int n = 0;
        for (size_t i = 0; i < deckSize; i++) {
            (n == 3) ? n = 1 : n++;
            if (deck[i] == n) {
                lost++;
                break;
            }
        }
    }

    steady_clock::time_point finish = steady_clock::now();
    nanoseconds elapsed = duration_cast<nanoseconds>(finish-start);

    double winratio = static_cast<double>(rounds - lost) / rounds;
    std::cout << std::setprecision(5)
              << " Played..............: " << rounds          << std::endl
              << " Lost................: " << lost            << std::endl
              << " Won.................: " << (rounds - lost) << std::endl
              << " Probability.........: " << winratio * 100  << '%' << std::endl
              << " Mean time per round.: " << elapsed.count()/rounds << " nanoseconds" << std::endl
              << " Total time elapsed..: " << elapsed.count()/(1000*1000) << " milliseconds" << std::endl;
    return 0;
}
