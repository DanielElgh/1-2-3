#include <cstdio>
#include <algorithm>
#include <ctime>
#include <cmath>
int randomizer(int i) { return std::rand() % i; }

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
    const unsigned int deckSize = sizeof(deck) / sizeof(*deck);
    std::srand(unsigned(std::time(0)));
    for (unsigned int played = 0; played < rounds; played++) {
        std::random_shuffle(&deck[0], &deck[deckSize], randomizer);
        unsigned int n = 0;
        for (unsigned int i = 0; i <= deckSize; i++) {
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
