#include <cstdio>
#include <cstdlib>
#include <ctime>

void shuffle_deck(unsigned int arr[], size_t size) {
    for (size_t i = 0; i < size; i++) {
        unsigned int tmp = arr[i];
        unsigned int rand = std::rand() % size;
        arr[i] = arr[rand];
        arr[rand] = tmp;
    }
}

int main() {
    const unsigned int rounds = 10000000U;
    unsigned int lost = 0;
    unsigned int deck[] = {
        1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0
    };
    const size_t deckSize = sizeof(deck) / sizeof(*deck);
    std::srand(unsigned(std::time(NULL)));
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
    double winrate = (double)(rounds - lost) / (double)rounds * 100;
    printf("Lost %d out of %d rounds.\nWinrate is %.4f%%\n", lost,
        rounds, winrate);
    return 0;
}
