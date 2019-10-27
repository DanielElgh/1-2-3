#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <thread>
#include <mutex>

int run(int t);

static std::mutex mtx;

static unsigned int g_seed;

inline void fast_srand(int seed) {
    g_seed = seed;
}

inline int fastrand() {
    g_seed = (214013*g_seed+2531011);
    return (g_seed>>16)&0x7FFF;
}

void shuffle_deck(unsigned int arr[], const size_t size) {
    unsigned int tmp, rand;
    #if __GNUC__ > 7
    #   pragma GCC unroll 52
    #endif
    #pragma GCC ivdep
    for (size_t i = 0; i != size; i++) {
        tmp = arr[i];
        rand = fastrand() % size;
        arr[i] = arr[rand];
        arr[rand] = tmp;
        /*
        i++; // Only shuffle every other card.
             // Increases performance of course, but at the cost of lower "suffle quality".
        */
    }
}

int main(int argc, char **argv) {
    int num_threads = std::thread::hardware_concurrency();

    // If started with arguments, expect it to be the number of threads to use.
    if (argc > 1)
        num_threads = atoi(argv[1]);

    // Substract number of threads to make room for the main thread.
    if (num_threads >= 2)
        num_threads--;

    std::thread *t = new std::thread[num_threads];

    // Launch all worker threads.
    for (int i = 0; i < num_threads; ++i) {
        t[i] = std::thread(run, fastrand()); // Use fastrand to generate seed for each thread.
    }

    std::cout << "played;lost;won;ratio;average ns per round;ms total" << std::endl;

    // Wait for all worker threads to finish and join.
    for (int i = 0; i < num_threads; ++i) {
        t[i].join();
    }

    return 0;
}

int run(int t) {
    using namespace std::chrono;
    const unsigned int timesToPlay = 10000000U;
    const size_t deckSize = 52;
    unsigned int lost = 0;
    unsigned int deck[deckSize] = {
        1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3
    };
    steady_clock::time_point start = steady_clock::now();

    fast_srand(static_cast<unsigned int>(std::time(NULL)) + t);
    for (unsigned int played = 0; played != timesToPlay; played++) {
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

    double winratio = static_cast<double>(timesToPlay - lost) / timesToPlay;
    mtx.lock(); // Set mutex lock to make make "thread-safe"
    std::cout << std::setprecision(5) << timesToPlay
              << ";" << lost
              << ";" << (timesToPlay - lost)
              << ";" << winratio * 100
              << ";" << elapsed.count()/timesToPlay
              << ";" << elapsed.count()/(1000*1000) << std::endl;
    mtx.unlock();
    return 0;
}
