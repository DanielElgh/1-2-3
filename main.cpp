#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>

void shuffle_deck(unsigned int arr[], const size_t size) {
    for (size_t i = 0; i < size; i++) {
        unsigned int tmp = arr[i];
        unsigned int rand = std::rand() % size;
        arr[i] = arr[rand];
        arr[rand] = tmp;
    }
}

int main() {
    const unsigned int rounds = 10000000U;
    const size_t deckSize = 52;
    unsigned int lost = 0;
    unsigned int deck[deckSize] = {
        1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3
    };
    std::srand(static_cast<int>(std::time(NULL)));
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
    double winratio = static_cast<double>(rounds - lost) / rounds;
    std::cout << std::setprecision(5)
              << " Played.........: " << rounds          << std::endl
              << " Lost...........: " << lost            << std::endl
              << " Won............: " << (rounds - lost) << std::endl
              << " Probability....: " << winratio * 100  << '%' << std::endl;

    return 0;
}
