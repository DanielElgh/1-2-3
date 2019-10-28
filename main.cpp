#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <thread>
#include <mutex>
#include <exception>
#include <cctype>
#include <sstream>
#include <string>

static const unsigned int TIMES_TO_PLAY = 10000000U;
static const size_t DECK_SIZE = 52;

static unsigned int g_seed;
static std::mutex mtx;

inline void fast_srand(int seed) {
    g_seed = seed;
}

inline int fastrand() {
    g_seed = (214013*g_seed+2531011);
    return (g_seed>>16)&0x7FFF;
}

void shuffle_deck(unsigned int arr[], const size_t size) {
    /**
     * This shuffle function iterates through the deck and swaps each card with a random card.
     * One card can be switched several times and even be "swiched" with itself.
     */
    unsigned int tmp, r;
    for (size_t i = 0; i != size; i++) {
        tmp = arr[i];           // 1. Memorize the card located at index i.
        r = fastrand() % size;  // 2. Randomize an index, from now on known as index r.
        arr[i] = arr[r];        // 3. Copy the card from index r and overwrite the card at index i.
        arr[r] = tmp;           // 4. Finalize the swap by overwriting the card at index r with the
                                //    original card from index i.
    }
}

bool str_is_digit (char *str) {
    /**
     * Make sure all characters in the string conform with cctype's implementation of isdigit().
     * Returns a boolean false if it does not.
     */
    // Step through a C-style string until the NULL terminator appear.
    while(*str) {
        if (!isdigit(*str)) {
            std::cerr << "Invalid character in number of worker threads" << std::endl;
            return false;
        }
        str++;
    }
    return true;
}

void worker(int seed) {
    using namespace std::chrono;
    unsigned int lost = 0;

    // Initialize the deck with all aces, twos, and threes.Leave the remaining 40 cards as zero.
    unsigned int deck[DECK_SIZE] = {
        1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3
    };

    // Get the current timestamp so that we later can measure elapsed time.
    steady_clock::time_point start = steady_clock::now();

    // Set thread specific random seed.
    fast_srand(static_cast<int>(std::time(NULL) + seed));

    // Play the game, repeatedly...
    for (unsigned int played = 0; played != TIMES_TO_PLAY; played++) {
        shuffle_deck(deck, DECK_SIZE);
        unsigned int n = 0;
        for (size_t i = 0; i != DECK_SIZE; i++) {
            (n == 3) ? n = 1 : n++;
            if (deck[i] == n) {
                lost++;
                break;
            }
        }
    }

    // Get timestamp again and calculate elapsed time.
    steady_clock::time_point finish = steady_clock::now();
    nanoseconds elapsed = duration_cast<nanoseconds>(finish-start);

    // Prepare an output string stream with all the statistics we'd like to print.
    double winratio = static_cast<double>(TIMES_TO_PLAY - lost) / TIMES_TO_PLAY;
    std::ostringstream oss;
    oss << std::setprecision(5) << TIMES_TO_PLAY
        << ";" << lost
        << ";" << (TIMES_TO_PLAY - lost)
        << ";" << winratio * 100
        << ";" << elapsed.count()/TIMES_TO_PLAY
        << ";" << elapsed.count()/(1000*1000) << std::endl;

    // Acquire a Mutex lock before quickly printing to stdout and releaseing the lock again.
    mtx.lock();
    std::cout << oss.str();
    mtx.unlock();

    // Terminate thread.
    return;
}

int main(int argc, char **argv) {
    signed int num_worker_threads;
    std::thread *p_thread_pool;

    /**
     * If program started with arguments, expect the first one to be the number of threads to use.
     * If not, try to detect number of supported concurrent threads using the std::thread function.
     * Note: Both conditions may set num_worker_threads to zero or a negative number. This is
     * dealt with right afterwards by setting values less than one (1) to one (1).
     */
    if (argc > 1 && str_is_digit(argv[1])) {
        num_worker_threads = atoi(argv[1]);
    } else {
        num_worker_threads = std::thread::hardware_concurrency() - 1;
    }

    if (num_worker_threads < 1) {
        num_worker_threads = 1;
    }

    // Seed the randomizer function with number derived from std::time().
    fast_srand(static_cast<int>(std::time(NULL)));

    // Keep track on all worker threads in a dynamically allocated array.
    try {
        p_thread_pool = new std::thread[num_worker_threads];
    } catch (std::exception& e) {
        // die if dynamic allocation failed, but first print error to stderr.
        std::cerr << "Failed to allocate thread pool: " << e.what() << std::endl;
        return 1;
    }

    // Print the CSV header before we dispatch our worker thread(s) to avoid a race condition.
    std::cout << "played;lost;won;winratio;average ns per round;ms total" << std::endl;

    // Dispatch the workers.
    for (int i = 0; i < num_worker_threads; i++) {
        p_thread_pool[i] = std::thread(worker, fastrand()); // Pass new random seed with each thread.
    }

    // Wait for all worker threads to finish.
    for (int i = 0; i < num_worker_threads; i++) {
        p_thread_pool[i].join();
    }

    return 0;
}
